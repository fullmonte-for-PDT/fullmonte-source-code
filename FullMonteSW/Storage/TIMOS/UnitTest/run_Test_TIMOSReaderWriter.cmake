MACRO(EXEC_CHECK CMD)
	EXECUTE_PROCESS(COMMAND ${CMD} RESULT_VARIABLE CMD_RESULT)
	IF(CMD_RESULT)
		MESSAGE(FATAL_ERROR "Error running ${CMD}")
	ENDIF()
ENDMACRO()
EXEC_CHECK(${CMD1})
EXEC_CHECK(${CMD2})
