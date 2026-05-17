#include <stdio.h>

int main()
{
    int n, cost[10][10], distance[10], visited[10], previous[10];
    int i, j, count, mindistance, selectedNode, src;

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
        previous[i] = src;
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
                selectedNode = i;
            }
        }

        visited[selectedNode] = 1;

        for (i = 0; i < n; i++)
        {
            if (!visited[i])
            {
                if (mindistance + cost[selectedNode][i] < distance[i])
                {
                    distance[i] = mindistance + cost[selectedNode][i];
                    previous[i] = selectedNode;
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
                j = previous[j];
                printf(" <- %d", j);

            } while (j != src);
        }
    }

    return 0;
}