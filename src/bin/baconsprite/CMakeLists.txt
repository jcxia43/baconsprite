add_executable(baconsprite
	Main.cpp
	Arguments.cpp
	Set.cpp
	Sprite.cpp
	Chunk.cpp
	Test.cpp
	CellSet.cpp
	CellCoverage.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}/lib
)

target_link_libraries(baconsprite
	gd
	jsoncpp
)

install(
	TARGETS baconsprite
	DESTINATION "bin"
)