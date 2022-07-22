/*
 * @file   eeprom.cpp
 * @author Mikhail Ezhov <ezhov93@gmail.com>
 * @brief  EEPROM source file.
 */

#include "eeprom.h"

#include "MDR32F9Qx_eeprom.h"
#include "MDR32F9Qx_rst_clk.h"

void Eeprom::begin(Bank blank) {
  _bank = blank;
  uint32_t cpu = SystemCoreClock / 1e6;
  RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
  if (cpu < 25)
    EEPROM_SetLatency(EEPROM_Latency_0);
  else if (cpu < 50)
    EEPROM_SetLatency(EEPROM_Latency_1);
  else if (cpu < 75)
    EEPROM_SetLatency(EEPROM_Latency_2);
  else if (cpu < 100)
    EEPROM_SetLatency(EEPROM_Latency_3);
  else if (cpu < 125)
    EEPROM_SetLatency(EEPROM_Latency_4);
  else if (cpu < 150)
    EEPROM_SetLatency(EEPROM_Latency_5);
  else
    EEPROM_SetLatency(EEPROM_Latency_6);
}

void Eeprom::begin() { begin(_bank); }

__RAMFUNC void Eeprom::format(void) {
  __disable_irq();
  EEPROM_EraseAllPages(_bank);
  __enable_irq();
}

__RAMFUNC void Eeprom::erase(Page page) {
  const uint32_t address = pageToAddress(page);
  __disable_irq();
  EEPROM_ErasePage(address, _bank);
  __enable_irq();
}

bool Eeprom::isEmpty() const {
  uint32_t size;
  switch (_bank) {
    case MainBank:
    case AllBanks:
      size = NumberPagesOfMainBank;
      break;
    case InfoBank:
      size = NumberPagesOfInfoBank;
      break;
  }
  for (uint32_t page = Page0; page < size; ++page)
    if (!isEmpty((Page)page)) return false;
  return true;
}

bool Eeprom::isEmpty(Page page) const {
  const uint32_t beginPageAddress = pageToAddress(page);
  const uint32_t endPageAddress = beginPageAddress + PageSize;
  for (uint32_t cnt = beginPageAddress; cnt < endPageAddress;
       cnt += sizeof(uint32_t))
    if (read(cnt) != PageEmpty) return false;
  return true;
}

__RAMFUNC uint32_t Eeprom::read(uint32_t address) const {
  __disable_irq();
  uint32_t data = EEPROM_ReadWord(address, _bank);
  __enable_irq();
  return data;
}

__RAMFUNC void Eeprom::read(uint32_t address, void *data, int size) {
  uint32_t *data_ptr = (uint32_t *)data;
  for (uint32_t cnt = 0; cnt < size; cnt += 4)
    if (cnt < NumberDWordOfPages) *data_ptr++ = read(address + cnt);
}

__RAMFUNC void Eeprom::write(uint32_t address, uint32_t data) {
  __disable_irq();
  erase(addressToPage(address));
  EEPROM_ProgramWord(address, _bank, data);
  __enable_irq();
}

__RAMFUNC void Eeprom::write(uint32_t address, void *data, int size) {
  uint32_t *data_ptr = (uint32_t *)data;
  __disable_irq();
  erase(addressToPage(address));
  for (uint32_t cnt = 0; cnt < size; cnt += 4)
    if (cnt < NumberDWordOfPages)
      EEPROM_ProgramWord(address + cnt, _bank, *data_ptr++);
  __enable_irq();
}

uint32_t Eeprom::pageToAddress(Page page) { return page << 12; }

Eeprom::Page Eeprom::addressToPage(uint32_t address) {
  return (Page)((address & PageMask) >> 12);
}

/*
void Eeprom::update(uint32 address, uint32 data)
{
  const uint32 beginPageAddr = address&PAGE_MASK;
  const uint32 page = ADDRESS_TO_PAGE(address);
  uint32 array[NUMBER_DWORD_OF_PAGE];







  read(address, array, PAGE_SIZE);
  array[address&(~PAGE_MASK)] = data;
  erase((Page)page);
  write(beginPageAddr, array, PAGE_SIZE);
}

void Eeprom::update(uint32 address, void* data, int size)
{
  const uint32 beginPageAddr = address&PAGE_MASK;
  const uint32 endPageAddr = (address+size)&PAGE_MASK;
  const uint32 beginPage = ADDRESS_TO_PAGE(beginPageAddr);
  const uint32 endPage = ADDRESS_TO_PAGE(endPageAddr)
  const int numberOfPages = endPage-beginPage;
  uint32 array[NUMBER_DWORD_OF_PAGE];












  for (uint32 page = beginPage; page<endPage; ++page) {
    const uint32 beginUpdateAddr = add
    read(address, array, PAGE_SIZE);
    erase((Page)page);
    format()
    write();
  }
}
*/
