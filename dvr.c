#include <stdio.h>

struct router
{
    unsigned cost[20];
    unsigned from[20];
} routingTable[10];

int main()
{
    int costmat[20][20];
    int routers;

    printf("Enter the number of routers: ");
    scanf("%d", &routers);

    printf("Enter the cost matrix:\n");
    for (int i = 0; i < routers; i++)
    {
        for (int j = 0; j < routers; j++)
        {
            scanf("%d", &costmat[i][j]);
            costmat[i][i] = 0;
            routingTable[i].cost[j] = costmat[i][j];
            routingTable[i].from[j] = j;
        }
    }

    int changed;
    do
    {
        changed = 0;
        for (int i = 0; i < routers; i++)
        {
            for (int j = 0; j < routers; j++)
            {
                for (int k = 0; k < routers; k++)
                {
                    if (costmat[i][k] >= 999)
                        continue;

                    if (routingTable[i].cost[j] > costmat[i][k] + routingTable[k].cost[j])
                    {
                        routingTable[i].cost[j] = costmat[i][k] + routingTable[k].cost[j];
                        routingTable[i].from[j] = k;
                        changed = 1;
                    }
                }
            }
        }
    } while (changed);

    for (int i = 0; i < routers; i++)
    {
        printf("\nFor router %d\n", i + 1);
        for (int j = 0; j < routers; j++)
        {
            printf("Router %d via %d distance %d\n",
                   j + 1,
                   routingTable[i].from[j] + 1,
                   routingTable[i].cost[j]);
        }
    }
    printf("\n");

    return 0;
}
