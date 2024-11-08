#include "at32f415.h"
#include "SystemInit.h"

#define SYSCLK_FREQ 48000000

void system_clock_config(void)
{
  /* config flash psr register */
  flash_psr_set(FLASH_WAIT_CYCLE_4);

  /* reset crm */
  crm_reset();

  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
  
  /* wait till hext is ready */
  while(crm_hext_stable_wait() == ERROR)
  {
  }


  /* config pll clock resource */
  //crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_18); //for 144000000
  crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_6); //for 48000000
  //crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_2); //for 16000000

  //crm_pll_config(CRM_PLL_SOURCE_HICK, CRM_PLL_MULT_6); //for 48000000

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
  {
  }

  /* config ahbclk */
  crm_ahb_div_set(CRM_AHB_DIV_1);

  /* config apb2clk */
  crm_apb2_div_set(CRM_APB2_DIV_2);

  /* config apb1clk */
  crm_apb1_div_set(CRM_APB1_DIV_2);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
  {
  }

  /* disable auto step mode */
  crm_auto_step_mode_enable(FALSE);

  /* update system_core_clock global variable */
  system_core_clock_update();

  systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);
}

void RCC_Clock_Periph(void)
{
  // AHB Periph 1 (SYSCLK_FREQ)
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);


  // APB 1 Periph (SYSCLK_FREQ/2)
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);  // Enable PWR clock
  
  crm_periph_clock_enable(CRM_I2C1_PERIPH_CLOCK, TRUE); // EEPROM
  crm_periph_clock_enable(CRM_SPI1_PERIPH_CLOCK, TRUE); // Flash

  crm_periph_clock_enable(CRM_TMR10_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);

}


void systemInit()
{
    system_clock_config();
    SysTick_Config(SYSCLK_FREQ/1000);
    RCC_Clock_Periph();
}



