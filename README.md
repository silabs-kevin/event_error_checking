1. Copy the folder "third_party", file <log.h> <erro_checking.c>.
2. Add below lines to main.c
	#include "log.h"
	...
	INIT_LOG();
	...
3. Modify the project settings, add ${workspace_loc:/${ProjName}/third_party/RTT} to include paths
4. Modify the hal-config.h VCOM symbol
5. Copy the retargetserial related files to the project.
