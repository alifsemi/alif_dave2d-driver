#ifndef __1_dave_irq_h_H
#define __1_dave_irq_h_H

/*---------------------------------------------------------------------------
*  Section: Interrupt control
*/

/*---------------------------------------------------------------------------
*   Function: d1_irc_enable
*
*   Enable D/AVE2D interrupts
*/
void d1_irq_enable();

/*---------------------------------------------------------------------------
*   Function: d1_irc_enable
*
*   Disable D/AVE2D interrupts
*/
void d1_irq_disable();

/*---------------------------------------------------------------------------
*   Function: d1_irc_enable
*
*   Disable D/AVE2D interrupts
*
*   Parameters:
*     context - pointer to d1_device context object
*
*/
void d1_set_isr_context( void *context );

#endif
