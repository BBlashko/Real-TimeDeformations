configure_file("${PROJECT_INCLUDE}/environment.hpp.in" "${PROJECT_INCLUDE}/environment.hpp")

set(PROJECT_INCLUDE_FILES
	"${PROJECT_INCLUDE}/deformationscene.hpp"
	"${PROJECT_INCLUDE}/imguiwrapper.hpp"
	"${PROJECT_INCLUDE}/image.hpp"
	"${PROJECT_INCLUDE}/handle.hpp"
	PARENT_SCOPE
)
