1. Copy the folder "third_party", file <log.h>.
2. Add below lines to main.c
	#include "log.h"
	...
	INIT_LOG();
	...
3. Modify the project settings, add ${workspace_loc:/${ProjName}/third_party/RTT} to include paths
   
