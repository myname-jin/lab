#include <stdio.h>  /* printf(), scanf()를 위해 추가 */
#include <stdlib.h> /* exit()를 위해 */

int main()
{
    int exit_status;

    printf("enter exit status: ");
    scanf("%d", &exit_status);
    exit(exit_status);
}
