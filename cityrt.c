
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CITIES 30
#define MAX_NAME_LEN 50

char cities[MAX_CITIES][MAX_NAME_LEN];
int graph[MAX_CITIES][MAX_CITIES];
int cityCount = 0;
int findCity(const char name[]);
int addCityInternal(const char name[]);
void addRoadInternal(const char a[], const char b[], int distance);
void readLine(char text[], int size);
int readInt(void);
void loadDefaultRoutes(void);
void saveData(void);
void loadData(void);
void addCity(void);
void addRoad(void);
void searchDistance(void);
void displayGraph(void);

int findCity(const char name[]) {
    for (int i = 0; i < cityCount; i++) {
        if (strcmp(cities[i], name) == 0)
            return i;
    }
    return -1;
}
int addCityInternal(const char name[]) {
    int existing = findCity(name);
    if (existing != -1)
        return existing;
    if (cityCount >= MAX_CITIES)
        return -1;
    strcpy(cities[cityCount], name);
    return cityCount++;
}
void addRoadInternal(const char a[],
                     const char b[], int distance) {
    int i = addCityInternal(a);
    int j = addCityInternal(b);
    if (i != -1 && j != -1 && i != j && distance > 0) {
        graph[i][j] = distance;
        graph[j][i] = distance;
    }
}
void readLine(char text[], int size) {
    if (fgets(text, size, stdin)) {
        text[strcspn(text, "\r\n")] = '\0';
    } else {
        text[0] = '\0';
    }
}
int readInt(void) {
    char line[50];
    char *end;
    long value;
    readLine(line, sizeof(line));
    value = strtol(line, &end, 10);
    if (end == line || *end != '\0' ||
        value < 0 || value > 2147483647L)
        return -1;
    return (int)value;
}
void loadDefaultRoutes(void) {
    addRoadInternal("Dehradun", "Delhi", 250);
    addRoadInternal("Dehradun", "Haridwar", 55);
    addRoadInternal("Dehradun", "Mussoorie", 35);
    addRoadInternal("Dehradun", "Rishikesh", 45);

    addRoadInternal("Delhi", "Jaipur", 280);
    addRoadInternal("Delhi", "Chandigarh", 245);
    addRoadInternal("Delhi", "Agra", 230);
    addRoadInternal("Delhi", "Lucknow", 550);

    addRoadInternal("Jaipur", "Udaipur", 395);
    addRoadInternal("Jaipur", "Jodhpur", 335);

    addRoadInternal("Chandigarh", "Shimla", 115);
    addRoadInternal("Chandigarh", "Amritsar", 230);

    addRoadInternal("Haridwar", "Rishikesh", 25);
    addRoadInternal("Lucknow", "Kanpur", 90);
    addRoadInternal("Agra", "Gwalior", 120);

    addRoadInternal("Mumbai", "Pune", 150);
    addRoadInternal("Mumbai", "Surat", 285);
    addRoadInternal("Pune", "Hyderabad", 560);

    addRoadInternal("Kolkata", "Patna", 585);
    addRoadInternal("Bengaluru", "Chennai", 350);
}
void saveData(void) {
    FILE *fp = fopen("citydata.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for saving!\n");
        return;
    }
    fprintf(fp, "%d\n", cityCount);
    for (int i = 0; i < cityCount; i++) {
        fprintf(fp, "%s\n", cities[i]);
    }
    for (int i = 0; i < cityCount; i++) {
        for (int j = i + 1; j < cityCount; j++) {
            if (graph[i][j] > 0) {
                fprintf(fp, "%d %d %d\n",
                        i, j, graph[i][j]);
            }
        }
    }
    fclose(fp);
    printf("Data saved successfully in citydata.txt!\n");
}
void loadData(void) {
    FILE *fp = fopen("citydata.txt", "r");
    if (fp == NULL) {
        printf("No saved data found.\n");
        printf("Loading default routes...\n");
        loadDefaultRoutes();
        saveData();
        return;
    }
    int count;
    if (fscanf(fp, "%d", &count) != 1 ||
        count < 0 || count > MAX_CITIES) {
        printf("Invalid saved data. Loading default routes.\n");
        fclose(fp);
        cityCount = 0;
        memset(graph, 0, sizeof(graph));
        loadDefaultRoutes();
        saveData();
        return;
    }
    cityCount = 0;
    memset(graph, 0, sizeof(graph));
    int ch;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF);
    for (int i = 0; i < count; i++) {
        if (fgets(cities[i], MAX_NAME_LEN, fp) == NULL) {
            printf("Error reading city names!\n");
            fclose(fp);
            cityCount = 0;
            memset(graph, 0, sizeof(graph));
            loadDefaultRoutes();
            saveData();
            return;
        }
        cities[i][strcspn(cities[i], "\r\n")] = '\0';
        if (strlen(cities[i]) == 0) {
            printf("Invalid city name!\n");
            fclose(fp);
            cityCount = 0;
            memset(graph, 0, sizeof(graph));
            loadDefaultRoutes();
            saveData();
            return;
        }
        for (int j = 0; j < i; j++) {
            if (strcmp(cities[i], cities[j]) == 0) {
                printf("Duplicate city found!\n");
                fclose(fp);
                cityCount = 0;
                memset(graph, 0, sizeof(graph));
                loadDefaultRoutes();
                saveData();
                return;
            }
        }
        cityCount++;
    }
    int a, b, distance;
    while (fscanf(fp, "%d %d %d",
                  &a, &b, &distance) == 3) {
        if (a >= 0 && a < cityCount &&
            b >= 0 && b < cityCount &&
            a != b && distance > 0) {
            graph[a][b] = distance;
            graph[b][a] = distance;
        }
    }
    fclose(fp);
    printf("Saved data loaded successfully!\n");
}
void addCity(void) {
    char name[MAX_NAME_LEN];
    printf("\nEnter city name: ");
    readLine(name, sizeof(name));

    if (strlen(name) == 0) {
        printf("City name cannot be empty!\n");
        return;
    }
    if (findCity(name) != -1) {
        printf("City already exists!\n");
        return;
    }
    if (addCityInternal(name) == -1) {
        printf("Maximum cities reached!\n");
        return;
    }
    printf("City added successfully!\n");
    saveData();
}
void addRoad(void) {
    char city1[MAX_NAME_LEN];
    char city2[MAX_NAME_LEN];
    int distance, i, j;

    printf("\nEnter first city: ");
    readLine(city1, sizeof(city1));
    printf("Enter second city: ");
    readLine(city2, sizeof(city2));

    i = findCity(city1);
    j = findCity(city2);
    if (i == -1 || j == -1) {
        printf("One or both cities not found!\n");
        printf("Please add the cities first.\n");
        return;
    }
    if (i == j) {
        printf("A city cannot connect to itself!\n");
        return;
    }
    printf("Enter distance in km: ");
    distance = readInt();
    if (distance <= 0) {
        printf("Invalid distance!\n");
        return;
    }
    graph[i][j] = distance;
    graph[j][i] = distance;
    printf("Road added successfully!\n");
    saveData();
}
void searchDistance(void) {
    char source[MAX_NAME_LEN];
    char destination[MAX_NAME_LEN];
    int i, j;
    printf("\nEnter starting city: ");
    readLine(source, sizeof(source));
    printf("Enter destination city: ");
    readLine(destination, sizeof(destination));
    i = findCity(source);
    j = findCity(destination);
    if (i == -1 || j == -1) {
        printf("City not found!\n");
        return;
    }
    if (i == j) {
        printf("Both cities are the same. Distance = 0 km\n");
        return;
    }
    if (graph[i][j] != 0) {
        printf("\nRoute: %s -> %s\n", source, destination);
        printf("Distance = %d km\n", graph[i][j]);
    } else {
        printf("No direct route found!\n");
    }
}
void displayGraph(void) {
    if (cityCount == 0) {
        printf("\nNo cities available.\n");
        return;
    }
    printf("\n===== CITY ROUTE MAP =====\n");
    for (int i = 0; i < cityCount; i++) {
        printf("\n%s -> ", cities[i]);
        int found = 0;
        for (int j = 0; j < cityCount; j++) {
            if (graph[i][j] != 0) {
                printf("%s (%d km)  ",
                       cities[j], graph[i][j]);
                found = 1;
            }
        }
        if (!found)
            printf("No direct roads");
    }
    printf("\n");
}
int main(void) {
    int choice;
    loadData();
    while (1) {
        printf("\n=CITY ROUTE FINDER=\n");
        printf("1. Add City\n");
        printf("2. Add Road\n");
        printf("3. Search Distance\n");
        printf("4. Display Route Map\n");
        printf("5. Display All Cities\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");
        choice = readInt();
        switch (choice) {
            case 1:
                addCity();
                break;
            case 2:
                addRoad();
                break;
            case 3:
                searchDistance();
                break;
            case 4:
                displayGraph();
                break;
            case 5:
                printf("\nAvailable Cities:\n");
                for (int i = 0; i < cityCount; i++) {
                    printf("%d. %s\n", i + 1, cities[i]);
                }
                break;
            case 6:
                printf("\nExiting City Route Finder.\n");
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
