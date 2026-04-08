const nodemailer = require("nodemailer");
const configModule = require("./config");

const transport = nodemailer.createTransport({
  host: "smtp.qq.com",
  port: 465,
  secure: true,
  auth: {
    user: configModule.emailUser,
    pass: configModule.emailPass
  }
});

function sendMail(mailOptions) {
  return new Promise((resolve, reject) => {
    transport.sendMail(mailOptions, (error, info) => {
      if (error) {
        console.log(error);
        reject(error);
      } else {
        console.log("mail sent:", info.response);
        resolve(info.response);
      }
    });
  });
}

module.exports.sendMail = sendMail;
