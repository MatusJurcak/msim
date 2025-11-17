ENTRY(_start)

MEMORY {
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
  RAM   (rwx): ORIGIN = 0xFFFF6000, LENGTH = 32K
}

SECTIONS {
  .vectors ORIGIN(FLASH) :
  {
    KEEP(*(.vectors))
  } > FLASH

  .text 0x400 :
  {
    *(.text*)
  } > FLASH
}