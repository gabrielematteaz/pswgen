# pswgen
Simple password generator with minimal customization

# Flags
Supported flags:
- 'u' > Minimum number of uppercase letters
- 'l' > Minimum number of lowercase letters
- 'd' > Minimum number of digits
- 's' > Minimum number of symbols (Used symbols: "~`! @#$%^&*()_-+={[}]|\:;"'<,>.?/");

# Example
Generate a password with 10 uppercase letters, no lowercase letters, 4 digits and 8 symbols
```shell
pswgen -u 10 -l 0 -d 4 -s 8
```
