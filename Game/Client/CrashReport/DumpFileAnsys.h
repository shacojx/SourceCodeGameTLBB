
#pragma once
#include <dbgeng.h>

void AnsysDumpFile(LPCTSTR szDumpFile);


class FileCallbacks : public IDebugOutputCallbacks
{
public:
    // IUnknown.
    STDMETHOD(QueryInterface)(
        THIS_
        IN REFIID InterfaceId,
        OUT PVOID* Interface
        );
    STDMETHOD_(ULONG, AddRef)(
        THIS
        );
    STDMETHOD_(ULONG, Release)(
        THIS
        );

    // IDebugOutputCallbacks.
    STDMETHOD(Output)(
        THIS_
        IN ULONG Mask,
        IN PCSTR Text
        );

public:
	void	SetOutputFile(FILE* fpOutput);

private:
	FILE*	m_fpOutput;
};