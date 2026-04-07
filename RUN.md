# 项目运行说明（简版）

本文档提供本项目三个模块的最小运行步骤：

- `gate_server`（C++ 网关服务）
- `front_end/XingChat`（Qt 客户端）
- `email_varify_service`（Node.js 邮件验证码服务）

---

## 1. 启动 gate_server

### 方式 A：使用 CLion/CMake

1. 打开 `gate_server` 工程
2. 选择 Debug 配置
3. Build 后运行 `gate_server`

默认监听端口：`8080`

### 方式 B：命令行

```bash
cmake -S gate_server -B gate_server/cmake-build-debug
cmake --build gate_server/cmake-build-debug -j
gate_server/cmake-build-debug/gate_server.exe
```

---

## 2. 启动 email_varify_service（可选）

目录：`email_varify_service`

1. 进入目录并安装依赖

```bash
cd email_varify_service
npm install
```

2. 修改 `config.json` 中邮箱配置：

- `email.user`：发件邮箱
- `email.pass`：SMTP 授权码

3. 启动服务

```bash
npm run serve
```

默认监听：`0.0.0.0:50051`

---

## 3. 启动 Qt 客户端 XingChat

目录：`front_end/XingChat`

1. 用 Qt Creator 打开 `XingChat.pro`
2. 先执行 qmake，再 Build
3. 运行程序

客户端会读取同目录 `config.ini`：

```ini
[GateServer]
host=localhost
port=8080
```

---

## 4. 快速联调验证

1. 先启动 `gate_server`
2. 启动 `XingChat`，进入注册页点击“获取验证码”
3. 或者用 curl 测试 POST：

```bash
curl.exe -X POST "http://127.0.0.1:8080/get_varifycode" -H "Content-Type: application/json" --data-raw "{\"email\":\"test@example.com\"}"
```

返回 `error: 0` 代表请求处理成功。
