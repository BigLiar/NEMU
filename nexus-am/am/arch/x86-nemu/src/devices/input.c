#include <am.h>
#include <x86.h>
#include <amdev.h>

#define KEYDOWN_MASK 0x8000
#define INPUT_PORT 0x60
size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
			uint32_t res = inl(INPUT_PORT);
      kbd->keydown = res & KEYDOWN_MASK ? 1 : 0;
      kbd->keycode = res & ~KEYDOWN_MASK;
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
