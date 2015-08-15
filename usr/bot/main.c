#include <kernel/sched.h>

#include "bot.h"

void main(void)
{
	new_task(&run_bot, 1, 0);
}
