# email_varify_service

Node.js HTTP email verification service with Redis TTL cache.

## 1) Install dependencies

```bash
npm install
```

## 2) Configure service

Edit `config.json`:

- `email.user`: sender mailbox (for example, 163 mailbox)
- `email.pass`: SMTP authorization code (not login password)
- `http.host` / `http.port`: service listening address
- `redis.host` / `redis.port` / `redis.password`: Redis connection

## 3) Start service

```bash
npm run serve
```

After start, it provides `POST /api/send_verify_code`.

## 4) Behavior

- Generate 6-digit verification code
- Send verification code by email
- Save verification code in Redis:
  - key: `code_<email>`
  - value: `<verify_code>`
  - ttl: `180s`
