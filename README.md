# Lily58 coding layout

A QWERTY layout designed for programming without requiring home-row mods or combos. It assumes the host uses a **US keyboard layout**; shifted symbol keycodes depend on that setting.

- **Base:** number row and familiar modifiers; the two inner keys are `[` and `]`. Thumbs, left to right: GUI, Alt, **SYM**, Space; Enter, **NAV**, Backspace, Delete.
- **SYM:** hold the left SYM thumb. The left home row has `` ` [ ] { } ``; the right home row has `: \ | = < >`. Other symbols are on the upper and lower rows. Top-left keys select Bluetooth profiles 1–5; the corner key clears the current profile's pairing.
- **NAV:** hold the right NAV thumb. H/J/K/L are Left/Down/Up/Right; Home, End, Page Up/Down and Insert are nearby. The top row is F1–F12. Left home-row keys provide GUI, Alt, Ctrl and Shift for modified navigation.
- The encoder binding, if an encoder is enabled and installed, adjusts volume.

Layers are momentary. Transparent keys retain their base-layer behavior. The layout intentionally removes the previous Java-specific macros and combos to avoid accidental activation while typing. Standard OS shortcuts use the dedicated modifiers; choose Ctrl or GUI according to your OS.

Build with the repository's GitHub Actions workflow; it produces firmware for both halves and a separate settings-reset image. Flash the appropriate left/right firmware to each half.
