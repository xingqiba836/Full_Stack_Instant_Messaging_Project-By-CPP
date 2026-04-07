const http = require("http");
const { v4: uuidv4 } = require("uuid");

const emailModule = require("./email");
const constModule = require("./const");
const configModule = require("./config");

async function sendVerifyCode(email) {
  console.log("email is", email);
  try {
    const appName = "XingChat";
    const uniqueId = uuidv4().replace(/-/g, "").slice(0, 6);
    const text =
      `【${appName}】邮箱验证码通知\n\n` +
      `您正在进行 ${appName} 账号注册。\n` +
      `本次验证码：${uniqueId}\n` +
      "有效期：3 分钟\n\n" +
      "若非本人操作，请忽略本邮件并注意账号安全。\n" +
      `此邮件由 ${appName} 系统自动发送，请勿直接回复。`;
    const html =
      `<div style="font-family:Arial,'Microsoft YaHei',sans-serif;line-height:1.7;color:#222;">` +
      `<h2 style="margin:0 0 12px;">${appName} 邮箱验证码通知</h2>` +
      `<p>您正在进行 <b>${appName}</b> 账号注册。</p>` +
      `<p>本次验证码：<span style="font-size:24px;font-weight:700;letter-spacing:2px;color:#0b57d0;">${uniqueId}</span></p>` +
      `<p>有效期：<b>3 分钟</b></p>` +
      `<hr style="border:none;border-top:1px solid #eee;margin:14px 0;" />` +
      `<p style="color:#666;">若非本人操作，请忽略本邮件并注意账号安全。</p>` +
      `<p style="color:#999;">此邮件由 ${appName} 系统自动发送，请勿直接回复。</p>` +
      `</div>`;

    const mailOptions = {
      from: configModule.emailUser,
      to: email,
      subject: `【${appName}】注册验证码`,
      text: text,
      html: html
    };

    const sendResult = await emailModule.sendMail(mailOptions);
    console.log("send result is", sendResult);

    return {
      email: email,
      error: constModule.Errors.Success
    };
  } catch (error) {
    console.log("send mail exception:", error);
    return {
      email: email,
      error: constModule.Errors.Exception
    };
  }
}

function main() {
  const server = http.createServer(async (req, res) => {
    if (req.method !== "POST" || req.url !== "/api/send_verify_code") {
      res.writeHead(404, { "Content-Type": "application/json" });
      res.end(JSON.stringify({ error: constModule.Errors.Exception, message: "not found" }));
      return;
    }

    let body = "";
    req.on("data", (chunk) => {
      body += chunk;
    });

    req.on("end", async () => {
      try {
        const payload = JSON.parse(body || "{}");
        const email = payload.email;
        if (!email) {
          res.writeHead(400, { "Content-Type": "application/json" });
          res.end(JSON.stringify({ error: constModule.Errors.Exception, message: "email is required" }));
          return;
        }

        const result = await sendVerifyCode(email);
        res.writeHead(200, { "Content-Type": "application/json" });
        res.end(JSON.stringify(result));
      } catch (error) {
        console.log("request parse exception:", error);
        res.writeHead(400, { "Content-Type": "application/json" });
        res.end(JSON.stringify({ error: constModule.Errors.Exception, message: "bad json" }));
      }
    });
  });

  server.listen(configModule.httpPort, configModule.httpHost, () => {
    console.log("http server started on", configModule.httpHost + ":" + configModule.httpPort);
  });
}

main();
