# GateServer 设计文档

## 1. 概述

`GateServer` 是 IM 项目的网关服务，主要职责是：

- 接收客户端 HTTP 请求
- 解析与路由请求到业务逻辑模块
- 调用下游服务（当前为 `email_varify_service` 的 HTTP 接口）
- 统一封装并返回响应

当前实现采用 `Boost.Asio + Boost.Beast` 异步网络模型，支持并发连接处理与下游 HTTP 连接池复用。

---

## 2. 总体架构

### 2.1 分层结构

- **接入层**
  - `HttpServer`
  - `HttpConnection`
- **业务路由层**
  - `LogicSystem`
- **下游调用层**
  - `VerifyEmailServiceClient`
- **基础设施层**
  - `AsioIOServicePool`
  - `ConfigMgr`
  - `Singleton`
  - `urlcode`

### 2.2 请求处理主链路

1. 客户端发送 HTTP 请求到 `GateServer`
2. `HttpServer` 监听端口并接收新连接
3. `HttpConnection` 异步读取请求并解析
4. `LogicSystem` 根据 URL 与 Method 分发到处理函数
5. 对于验证码请求，`VerifyEmailServiceClient` 调用邮件服务 HTTP API
6. 处理结果回写至 `HttpConnection::_response` 并异步发送给客户端

---

## 3. 核心模块设计

## 3.1 main.cpp

职责：

- 读取配置（端口）
- 创建主 `io_context`
- 初始化并启动 `HttpServer`
- 注册进程信号（`SIGINT`、`SIGTERM`）用于优雅退出
- 停止时调用 `AsioIOServicePool::Stop()`

---

## 3.2 HttpServer（`CServer.h/.cpp`）

职责：

- 创建 `acceptor` 监听网关端口
- 使用 `async_accept` 异步接收连接
- 每次接入时从 `AsioIOServicePool` 获取一个 `io_context`
- 基于该 `io_context` 创建 `HttpConnection`，实现连接处理线程分摊

特点：

- 接入层不处理业务
- 错误后继续监听，保证服务可持续接入

---

## 3.3 HttpConnection（`HttpConnection.h/.cpp`）

职责：

- 维护单连接生命周期
- 异步读取请求（`async_read`）
- 解析 URL 与 Query 参数（GET）
- 区分 Method（GET/POST/其他）
- 调用 `LogicSystem` 进行业务处理
- 生成并异步写回响应（`async_write`）
- 通过 deadline timer 控制连接处理超时

关键成员：

- `_request` / `_response`
- `_buffer`
- `deadline_`
- `_get_url` / `_get_params`

---

## 3.4 LogicSystem（`LogicSystem.h/.cpp`）

职责：

- 注册并维护 GET/POST 路由处理器
- 对外提供统一入口：
  - `HandleGetRequest()`
  - `HandlePostRequest()`

当前关键路由：

- `POST /get_varifycode`
  - 解析请求体 JSON
  - 提取 `email`
  - 调用 `VerifyEmailServiceClient::GetVerifyCode()`
  - 返回 `{ error, email }`

---

## 3.5 VerifyEmailServiceClient（`VerifyEmailServiceClient.h/.cpp`）

职责：

- 封装到 `email_varify_service` 的 HTTP 调用
- 承担连接池管理、超时、连接重建等稳定性逻辑

连接池设计：

- 内部维护固定大小连接池（`HttpPoolSize`）
- 连接对象包含：
  - `io_context`
  - `resolver`
  - `tcp_stream`
  - `connected` 状态
  - `request_count` 请求计数
- 通过 `AcquireConnection()` / `ReturnConnection()` 借还连接

可靠性策略：

- **连接可用性检查**：断开后自动重连
- **超时控制**：
  - `ConnectTimeoutSeconds`
  - `IoTimeoutSeconds`（读写）
- **连接重建策略**：
  - 单连接请求数达到 `MaxRequestsPerConnection` 后主动重建

---

## 3.6 AsioIOServicePool（`AsioIOServicePool.h/.cpp`）

职责：

- 创建多个 `io_context` 与工作线程
- 提供 round-robin 的 `GetIOService()` 分发
- 统一停止所有 `io_context` 并回收线程

说明：

- 使用 work guard 保持 `io_context.run()` 常驻
- 用于提高接入侧连接处理并发能力

---

## 3.7 ConfigMgr（`ConfigMgr.h/.cpp`）

职责：

- 启动时读取 `config.ini`
- 以 `section/key` 方式提供配置访问能力

当前关键配置：

- `GateServer.Port`
- `VarifyServer.Host`
- `VarifyServer.Port`
- `VarifyServer.Path`
- `VarifyServer.HttpPoolSize`
- `VarifyServer.MaxRequestsPerConnection`
- `VarifyServer.ConnectTimeoutSeconds`
- `VarifyServer.IoTimeoutSeconds`

---

## 4. 关键配置示例

文件：`gate_server/config.ini`

```ini
[GateServer]
Port=8080

[VarifyServer]
Host=127.0.0.1
Port=5000
Path=/api/send_verify_code
HttpPoolSize=4
MaxRequestsPerConnection=100
ConnectTimeoutSeconds=2
IoTimeoutSeconds=3
```

---

## 5. 已实现优化点

- 基于 `io_context` 线程池的连接处理并行化
- 下游邮件服务 HTTP 连接池复用
- 连接超时控制（connect/read/write）
- 单连接请求阈值后的主动重建
- 配置化管理（无需硬编码）

---

## 6. 后续可扩展方向

- 增加连接池监控指标（空闲连接、重连次数、超时次数）
- 为 `LogicSystem` 增加中间件能力（鉴权、限流、日志追踪）
- 引入服务发现（如 ZooKeeper/Consul）支持邮件服务多实例
- 引入熔断与重试策略，增强下游异常场景容错
- 提供统一错误码体系与结构化日志

---

## 7. 启动与联调建议

1. 启动 `email_varify_service`（HTTP 5000）
2. 启动 `gate_server`
3. 客户端调用 `POST /get_varifycode` 验证链路
4. 观察网关日志与邮件服务日志确认端到端成功

