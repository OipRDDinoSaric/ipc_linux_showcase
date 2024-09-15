# Declaring libraries as system suppresses the warnings from them.
function(declare_system_library target)
    message(STATUS "Declaring ${target} as system library.")
    get_target_property(target_aliased_name ${target} ALIASED_TARGET)
    if (target_aliased_name)
        set(target ${target_aliased_name})
    endif()
    set_target_properties(${target} PROPERTIES
            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:${target},INTERFACE_INCLUDE_DIRECTORIES>)
endfunction()