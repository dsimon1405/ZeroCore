#pragma once

struct ZC_ShProg
{
    unsigned int id;

    ZC_ShProg(unsigned int idV, unsigned int idF, unsigned int idG);

	ZC_ShProg(const ZC_ShProg&) = delete;
	ZC_ShProg& operator = (const ZC_ShProg&) = delete;
	
	ZC_ShProg(ZC_ShProg&& shader) noexcept;
	ZC_ShProg& operator = (ZC_ShProg&& shader);

    ~ZC_ShProg();

	/*
	Activate shader program.
	*/
    void ActivateOpenGL() const;

	int GetUniformLocation(const char* name);
};