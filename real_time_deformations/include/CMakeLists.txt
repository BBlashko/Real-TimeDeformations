configure_file("${PROJECT_INCLUDE}/environment.hpp.in" "${PROJECT_INCLUDE}/environment.hpp")

set(PROJECT_INCLUDE_FILES
	"${PROJECT_INCLUDE}/deformationscene.hpp"
	"${PROJECT_INCLUDE}/handle.hpp"
	"${PROJECT_INCLUDE}/pointhandle.hpp"
	PARENT_SCOPE
)
