ak#include <stdio.h>
#include <string.h>

struct Person{
    char name[50];
    int citNo;
    float salary;
};

int main(){
    struct Person person1;

    strcpy(person1.name, "George Orwell");

    person1.citNo = 1984;
    person1.salary = 2500;

    printf("Name: %s\n", person1.name);
    printf("Citizenship no: %d\n", person1.citNo);
    printf("Salary: %.2f\n", person1.salary);

    return 0;
}