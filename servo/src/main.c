#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <drivers/pwm.h>
#include <kernel.h>

#define DT_ALIAS_PWM_0_LABEL "PWM_2"

#ifndef DT_ALIAS_PWM_0_LABEL
#error "PWM device label not defined"
#endif

/* period of servo motor signal ->  20ms (50Hz) */
#define PERIOD (USEC_PER_SEC / 50U)

/* all in micro second */
#define STEP 100			  /* PWM pulse step */
#define MINPULSEWIDTH 1000	  /* Servo 0 degrees */
#define MIDDLEPULSEWIDTH 1500 /* Servo 90 degrees */
#define MAXPULSEWIDTH 2000	  /* Servo 180 degrees */
#define SLEEP_TIME_S 1		  /* Pause time */

/** 
 * Function to change servo position by STEP value.
 * 
 *  @param[in, out] dir pointer to direction variable.
 *  @param[in, out] pulse_width pointer to variable with pulse width value.
 */
void PWM_control(u8_t *dir, u32_t *pulse_width)
{
	if (*dir == 1U)
	{
		if (*pulse_width < MAXPULSEWIDTH)
		{
			*pulse_width += STEP;
		}
		else
		{
			*dir = 0U;
		}
	}
	else if (*dir == 0U)
	{
		if (*pulse_width > MINPULSEWIDTH)
		{
			*pulse_width -= STEP;
		}
		else
		{
			*dir = 1U;
		}
	}
}

/**
 *  Function to change servo position to one of three available in sequence: 0->90->180->90->0->... 
 * 
 * @param[in, out] dir pointer to direction variable.
 * @param[in, out] pulse_width pointer to variable with pulse width value.
 */
void PWM_position(u8_t *dir, u32_t *pulse_width)
{
	if (*dir == 0U)
	{
		*pulse_width = MINPULSEWIDTH;
		*dir = 1U;
	}
	else if (*dir == 1U)
	{
		*pulse_width = MIDDLEPULSEWIDTH;
		*dir = 2U;
	}
	else if (*dir == 2U)
	{
		*pulse_width = MAXPULSEWIDTH;
		*dir = 3U;
	}
	else if (*dir == 3U)
	{
		*pulse_width = MIDDLEPULSEWIDTH;
		*dir = 0U;
	}
}

void main(void)
{
	printk("Servo control program\n");

	struct device *pwm_dev;
	
	/* Set PWM starting positions as 0 degrees */
	u32_t pulse_width = MINPULSEWIDTH;
	u8_t dir = 0U;

	pwm_dev = device_get_binding(DT_ALIAS_PWM_0_LABEL);
	if (!pwm_dev)
	{
		printk("Cannot find PWM device!\n");
		return;
	}
	else
	{
		printk("PWM device find\n");
		printk("%s\n", DT_ALIAS_PWM_0_LABEL);
	}

	while (1)
	{
		printk("PWM device cycle\n");

		if (pwm_pin_set_usec(pwm_dev, 1, PERIOD, pulse_width, 0))
		{
			printk("PWM pin set fails\n");
			return;
		}

		printk("PWM pulse width: %d\n", pulse_width);

		printk("Current direction: %d\n", dir);

		PWM_control(&dir, &pulse_width);

		//PWM_position(&dir, &pulse_width);

		printk("Set direction %d\n\n", dir);

		k_sleep(K_SECONDS(SLEEP_TIME_S));
	}
}
