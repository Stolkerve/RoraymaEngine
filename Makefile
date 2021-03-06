# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  GLFW_config = debug
  Glad_config = debug
  ImGui_config = debug
  yaml_cpp_config = debug
  Engine_config = debug
  Editor_config = debug

else ifeq ($(config),release)
  GLFW_config = release
  Glad_config = release
  ImGui_config = release
  yaml_cpp_config = release
  Engine_config = release
  Editor_config = release

else ifeq ($(config),dist)
  GLFW_config = dist
  Glad_config = dist
  ImGui_config = dist
  yaml_cpp_config = dist
  Engine_config = dist
  Editor_config = dist

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := GLFW Glad ImGui yaml-cpp Engine Editor

.PHONY: all clean help $(PROJECTS) Dependencies

all: $(PROJECTS)

Dependencies: GLFW Glad ImGui yaml-cpp

GLFW:
ifneq (,$(GLFW_config))
	@echo "==== Building GLFW ($(GLFW_config)) ===="
	@${MAKE} --no-print-directory -C Engine/vendor/GLFW -f Makefile config=$(GLFW_config)
endif

Glad:
ifneq (,$(Glad_config))
	@echo "==== Building Glad ($(Glad_config)) ===="
	@${MAKE} --no-print-directory -C Engine/vendor/Glad -f Makefile config=$(Glad_config)
endif

ImGui:
ifneq (,$(ImGui_config))
	@echo "==== Building ImGui ($(ImGui_config)) ===="
	@${MAKE} --no-print-directory -C Engine/vendor/imgui -f Makefile config=$(ImGui_config)
endif

yaml-cpp:
ifneq (,$(yaml_cpp_config))
	@echo "==== Building yaml-cpp ($(yaml_cpp_config)) ===="
	@${MAKE} --no-print-directory -C Engine/vendor/yaml-cpp -f Makefile config=$(yaml_cpp_config)
endif

Engine: GLFW Glad ImGui yaml-cpp
ifneq (,$(Engine_config))
	@echo "==== Building Engine ($(Engine_config)) ===="
	@${MAKE} --no-print-directory -C Engine -f Makefile config=$(Engine_config)
endif

Editor: Engine GLFW Glad ImGui yaml-cpp
ifneq (,$(Editor_config))
	@echo "==== Building Editor ($(Editor_config)) ===="
	@${MAKE} --no-print-directory -C Editor -f Makefile config=$(Editor_config)
endif

clean:
	@${MAKE} --no-print-directory -C Engine/vendor/GLFW -f Makefile clean
	@${MAKE} --no-print-directory -C Engine/vendor/Glad -f Makefile clean
	@${MAKE} --no-print-directory -C Engine/vendor/imgui -f Makefile clean
	@${MAKE} --no-print-directory -C Engine/vendor/yaml-cpp -f Makefile clean
	@${MAKE} --no-print-directory -C Engine -f Makefile clean
	@${MAKE} --no-print-directory -C Editor -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo "  dist"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   GLFW"
	@echo "   Glad"
	@echo "   ImGui"
	@echo "   yaml-cpp"
	@echo "   Engine"
	@echo "   Editor"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"