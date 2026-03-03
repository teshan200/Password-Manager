# Password Manager (C)

A simple console-based password manager written in C.

It lets you save, view, search, modify, and delete account credentials (website, username, password), then persists them to a local file (`vault.txt`).

## Features

- Add a new account (unique website)
- Display all saved accounts
- Search account by website
- Modify existing account
- Delete account
- Count total saved accounts
- Find account with the longest password
- Find account with the shortest password
- Count websites containing a substring
- Detect weak passwords (length `< 6`)
- Colored terminal menu (ANSI colors; enabled on Windows where supported)

## Project Structure

- `password_vault.c` — main C source code
- `vault.txt` — local storage file for account data

## How Data Is Stored

- Data is saved in `vault.txt` in this format:

  ```
  website|username|encryptedPassword
  ```

- Passwords are obfuscated using a simple XOR operation with key `#` before being written.

## Build & Run

### Windows (GCC / MinGW)

```bash
gcc password_vault.c -o password_vault.exe
password_vault.exe
```

### Linux / macOS (GCC)

```bash
gcc password_vault.c -o password_vault
./password_vault
```

## Usage

1. Run the program.
2. Choose an option from the menu (`0` to `10`).
3. Manage your saved account entries.
4. On each add/modify/delete operation, data is automatically saved to `vault.txt`.

## Limits

Current compile-time limits in source:

- Maximum accounts: `20`
- Maximum website length: `60`
- Maximum username length: `60`
- Maximum password length: `60`

## Important Security Note

This project is best for learning/practice. XOR obfuscation is **not secure encryption** for real-world password protection.

For production-grade security, use:

- Strong encryption (e.g., AES-GCM)
- A master password with key derivation (PBKDF2/Argon2)
- Secure memory handling
- OS-protected key storage

## License

No license file is currently included. Add one (for example, MIT) if you plan to share this project publicly.
