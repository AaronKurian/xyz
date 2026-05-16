#include <stdio.h>

int main()
{
    int n, cost[10][10], distance[10], visited[10], pred[10];
    int i, j, count, mindistance, nextnode, src;

    printf("Enter the number of nodes: ");
    scanf("%d", &n);

    printf("Enter the cost matrix:\n");

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            scanf("%d", &cost[i][j]);

            if (cost[i][j] == 0)
                cost[i][j] = 999;
        }
    }

    printf("Enter the source node: ");
    scanf("%d", &src);

    for (i = 0; i < n; i++)
    {
        distance[i] = cost[src][i];
        pred[i] = src;
        visited[i] = 0;
    }

    distance[src] = 0;
    visited[src] = 1;
    count = 1;

    while (count < n - 1)
    {
        mindistance = 999;

        for (i = 0; i < n; i++)
        {
            if (distance[i] < mindistance && !visited[i])
            {
                mindistance = distance[i];
                nextnode = i;
            }
        }

        visited[nextnode] = 1;

        for (i = 0; i < n; i++)
        {
            if (!visited[i])
            {
                if (mindistance + cost[nextnode][i] < distance[i])
                {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
            }
        }

        count++;
    }

    for (i = 0; i < n; i++)
    {
        if (i != src)
        {
            printf("\nDistance of node %d = %d", i, distance[i]);

            printf("\nPath = %d", i);

            j = i;

            do
            {
                j = pred[j];
                printf(" <- %d", j);

            } while (j != src);
        }
    }

    return 0;
}