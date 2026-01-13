# Security Policy

## Supported Versions

| Version       | Supported          |
|---------------|--------------------|
| main branch   | :white_check_mark: |
| Other branches| :x:                |
| Previous tags | :x:                |

## Reporting a Vulnerability

If you discover a security vulnerability in this project, please report it responsibly:

1. **For sensitive disclosures**: Use [GitHub's Security Advisory](https://github.com/NanashiTheNameless/CaiXianlinRemoteIDMonitor/security/advisories)
2. **For general issues**: Open an [Issue](https://github.com/NanashiTheNameless/CaiXianlinRemoteIDMonitor/issues)
3. **For fixes**: Submit a [Pull Request](https://github.com/NanashiTheNameless/CaiXianlinRemoteIDMonitor/pulls)

Please note: This is a free, open-source hobby project with no bug bounty program.

## Security Considerations

This project is an Arduino-based RF receiver monitor for educational and personal use. Users should be aware of:

- **RF Interference**: 433MHz RF signals can be intercepted or spoofed by other devices on the same frequency
- **No Encryption**: The 433MHz protocol used includes no encryption or authentication mechanisms
- **No Validation**: The receiver accepts any valid-format messages on the monitored frequency
- **Hardware Security**: Ensure your Arduino and RF receiver module are physically secure if deployed in sensitive environments

## Dependencies

This project includes:

- **Arduino Platform**: LGPL-licensed components
- **Main Code**: BSD 2-Clause License
- **Custom Modified Libraries** (`libraries/`): BSD 2-Clause License

For security updates and advisories regarding Arduino or other dependencies, refer to their respective repositories and security policies.
