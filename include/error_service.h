#pragma once
#include <stdbool.h>

typedef struct {
    char title[256];
    char message[512];
    bool isError;
} ErrorState;

void ReportCriticalError(const char* title, const char* message);
bool HasGlobalError(void);
ErrorState* GetGlobalErrorState(void);
void ClearGlobalError(void);
bool ShowErrorDialog(void);