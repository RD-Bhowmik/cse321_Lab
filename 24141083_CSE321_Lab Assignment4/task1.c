#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 3
#define MAX_RESOURCES 3
#define MAX_NAME_LEN 20

typedef enum{ 
    READ = 1,
    WRITE = 2,
    EXECUTE = 4
}Permission;

typedef struct{
    char u_name[MAX_NAME_LEN];
}User;

typedef struct{
    char r_name[MAX_NAME_LEN];
}Resource;

typedef struct{
    char acl_user[MAX_NAME_LEN];
    int acl_permissions;
}ACLEntry;

typedef struct{
    Resource resource;
    ACLEntry entries[MAX_USERS];
    int e_count;
}ACLControlledResource;

typedef struct{
    char resourceName[MAX_NAME_LEN];
    int cap_permissions;
}Capability;

typedef struct{
    User user;
    Capability capabilities[MAX_RESOURCES];
    int capa_count;
}CapabilityUser;

void printPermissionss(int perm){
    char output[50] = "";
    int first = 1;
    int i = 0;
    int cur_bit = 1; 
    

    while(i < 3) {
        if(perm & cur_bit) {
            if(!first) {
                strcat(output, " ");
            }
            switch(i) {
                case 0: strcat(output, "Read"); break;
                case 1: strcat(output, "Write"); break;
                case 2: strcat(output, "Execute"); break;
            }
            first = 0;
        }
        cur_bit = cur_bit << 1; 
        i++;
    }


    if(first) { 
        strcpy(output, "None");
    }
    printf("%s", output);
}



int hasPermission(int userPerm, int requiredPerm){
    return ((userPerm & requiredPerm) ^ requiredPerm) == 0;
}

void checkACLAccess(ACLControlledResource *res, const char *userName, int perm){
    char output[200];
    char name_buff[20];
    int i = 0;
    int found = 0;

    switch(perm) {
        case READ: strcpy(name_buff, "READ"); break;
        case WRITE: strcpy(name_buff, "WRITE"); break;
        case EXECUTE: strcpy(name_buff, "EXECUTE"); break;
        default: 
            name_buff[0] = '\0';
            char tempBuf[50];
            sprintf(tempBuf, "%d", perm);
            strcpy(name_buff, tempBuf);
            break;
    }
    
    sprintf(output, "ACL Check: User %s requests %s on %s: Access ", userName, name_buff, (*res).resource.r_name);

    
    while(i < (*res).e_count){
        if(strcmp((*res).entries[i].acl_user, userName) == 0){
            found = 1;
            if(hasPermission((*res).entries[i].acl_permissions, perm)){
                strcat(output, "GRANTED\n");
            } 
            else {
                strcat(output, "DENIED\n");
            }
            printf("%s", output);
            return;
        }
        i++;
    }
    
    if(!found){
        sprintf(output, "ACL Check: User %s has NO entry for resource %s: Access DENIED\n", userName, (*res).resource.r_name);
        printf("%s", output);
    }
}


void checkCapabilityAccess(CapabilityUser *user, const char *resourceName, int perm){
    int i = 0;
    int found = 0;
    
    while(i < (*user).capa_count){
        if(strcmp((*user).capabilities[i].resourceName, resourceName) == 0){
            found = 1;
            printf("Capability Check: User %s requests ", (*user).user.u_name);
            
            switch(perm) {
                case READ: printf("READ"); break;
                case WRITE: printf("WRITE"); break;
                case EXECUTE: printf("EXECUTE"); break;
                default: printPermissionss(perm); break;
            }

            printf(" on %s: Access ", resourceName);
            


            if(hasPermission((*user).capabilities[i].cap_permissions, perm)){
                printf("GRANTED\n");
            } 
            else {
                printf("DENIED\n");
            }
            return;
        }
        i++;
    }
    
    if(!found){
        printf("Capability Check: User %s has NO capability for %s: Access DENIED\n", (*user).user.u_name, resourceName);
    }
}


int main(){
    User users[MAX_USERS] = {{"Alice"}, {"Bob"}, {"Charlie"}};
    Resource resources[MAX_RESOURCES] = {{"File1"}, {"File2"}, {"File3"}};
    ACLControlledResource aclResources[MAX_RESOURCES];
    
    

    aclResources[0].resource = resources[0]; 
    aclResources[0].e_count = 2;
    
    strcpy(aclResources[0].entries[0].acl_user, "Alice");
    aclResources[0].entries[0].acl_permissions = READ | WRITE;
    
    strcpy(aclResources[0].entries[1].acl_user, "Bob");
    aclResources[0].entries[1].acl_permissions = READ;
    
    aclResources[1].resource = resources[1]; 
    aclResources[1].e_count = 1;
    
    strcpy(aclResources[1].entries[0].acl_user, "Bob");
    aclResources[1].entries[0].acl_permissions = READ | WRITE;
    
    aclResources[2].resource = resources[2]; 
    aclResources[2].e_count = 2;
    
    strcpy(aclResources[2].entries[0].acl_user, "Alice");
    aclResources[2].entries[0].acl_permissions = EXECUTE;
    
    strcpy(aclResources[2].entries[1].acl_user, "Charlie");
    aclResources[2].entries[1].acl_permissions = READ | EXECUTE;

    CapabilityUser capUsers[MAX_USERS];
    
    capUsers[0].user = users[0]; 
    capUsers[0].capa_count = 2;
    
    strcpy(capUsers[0].capabilities[0].resourceName, "File1");
    capUsers[0].capabilities[0].cap_permissions = READ | WRITE;
    
    strcpy(capUsers[0].capabilities[1].resourceName, "File3");
    capUsers[0].capabilities[1].cap_permissions = EXECUTE;
    
    capUsers[1].user = users[1]; 
    capUsers[1].capa_count = 2;
    
    strcpy(capUsers[1].capabilities[0].resourceName, "File1");
    capUsers[1].capabilities[0].cap_permissions = READ;
    
    strcpy(capUsers[1].capabilities[1].resourceName, "File2");
    capUsers[1].capabilities[1].cap_permissions = READ | WRITE;
    capUsers[2].user = users[2]; 
    capUsers[2].capa_count = 1;
    
    strcpy(capUsers[2].capabilities[0].resourceName, "File3");
    capUsers[2].capabilities[0].cap_permissions = READ | EXECUTE;




    checkACLAccess(&aclResources[0], "Alice", READ); 
    checkACLAccess(&aclResources[0], "Bob", WRITE);  
    checkACLAccess(&aclResources[0], "Charlie", READ); 
    checkCapabilityAccess(&capUsers[0], "File1", WRITE); 
    checkCapabilityAccess(&capUsers[1], "File1", WRITE); 
    checkCapabilityAccess(&capUsers[2], "File2", READ);  

    return 0;
}