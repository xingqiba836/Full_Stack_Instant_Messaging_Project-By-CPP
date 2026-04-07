# email_varify_service

Node.js gRPC email verification service (day08 style).

## 1) Install dependencies

```bash
npm install
```

## 2) Configure email account

Edit `config.json`:

- `email.user`: sender mailbox (for example, 163 mailbox)
- `email.pass`: SMTP authorization code (not login password)
- `grpc.host` / `grpc.port`: service listening address

## 3) Start service

```bash
npm run serve
```

After start, it provides `VarifyService.GetVarifyCode`.
