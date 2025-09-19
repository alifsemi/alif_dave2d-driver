if (CONFIG_DAVE2D)
  set(DAVE2D_DIR ${ZEPHYR_DAVE2D_MODULE_DIR})

  zephyr_interface_library_named(DAVE2D)
  zephyr_library()
  zephyr_include_directories(
    ${DAVE2D_DIR}/zephyr
    ${DAVE2D_DIR}/d0/inc
    ${DAVE2D_DIR}/d1/inc
    ${DAVE2D_DIR}/d2/inc
  )

  zephyr_library_sources(
    ${DAVE2D_DIR}/d1/src/dave_base.c
    ${DAVE2D_DIR}/d1/src/dave_irq.c
    ${DAVE2D_DIR}/d1/src/dave_memory.c
  )

  zephyr_library_sources(
    ${DAVE2D_DIR}/d2/src/dave_64bitoperation.c
    ${DAVE2D_DIR}/d2/src/dave_blit.c
    ${DAVE2D_DIR}/d2/src/dave_box.c
    ${DAVE2D_DIR}/d2/src/dave_box.h
    ${DAVE2D_DIR}/d2/src/dave_circle.c
    ${DAVE2D_DIR}/d2/src/dave_context.c
    ${DAVE2D_DIR}/d2/src/dave_curve.c
    ${DAVE2D_DIR}/d2/src/dave_curve.h
    ${DAVE2D_DIR}/d2/src/dave_dlist.c
    ${DAVE2D_DIR}/d2/src/dave_driver.c
    ${DAVE2D_DIR}/d2/src/dave_edge.c
    ${DAVE2D_DIR}/d2/src/dave_edge.h
    ${DAVE2D_DIR}/d2/src/dave_errorcodes.c
    ${DAVE2D_DIR}/d2/src/dave_gradient.c
    ${DAVE2D_DIR}/d2/src/dave_hardware.c
    ${DAVE2D_DIR}/d2/src/dave_intern.h
    ${DAVE2D_DIR}/d2/src/dave_line.c
    ${DAVE2D_DIR}/d2/src/dave_line.h
    ${DAVE2D_DIR}/d2/src/dave_math.c
    ${DAVE2D_DIR}/d2/src/dave_memory.c
    ${DAVE2D_DIR}/d2/src/dave_pattern.c
    ${DAVE2D_DIR}/d2/src/dave_pattern.h
    ${DAVE2D_DIR}/d2/src/dave_perfcount.c
    ${DAVE2D_DIR}/d2/src/dave_polyline.c
    ${DAVE2D_DIR}/d2/src/dave_quad.c
    ${DAVE2D_DIR}/d2/src/dave_quad.h
    ${DAVE2D_DIR}/d2/src/dave_rbuffer.c
    ${DAVE2D_DIR}/d2/src/dave_render.c
    ${DAVE2D_DIR}/d2/src/dave_texture.c
    ${DAVE2D_DIR}/d2/src/dave_triangle.c
    ${DAVE2D_DIR}/d2/src/dave_utility.c
    ${DAVE2D_DIR}/d2/src/dave_viewport.c
    ${DAVE2D_DIR}/d2/src/dave_wedge.c
  )

  zephyr_library_sources_ifdef(
    CONFIG_WITH_MM_FIXED_RANGE
    ${DAVE2D_DIR}/d0/src/dave_d0lib.c
    ${DAVE2D_DIR}/d0/src/dave_d0_mm_fixed_range.c
  )

  zephyr_library_sources_ifdef(
    CONFIG_WITH_MM_FIXED_RANGE_FIXED_BLKCNT
    ${DAVE2D_DIR}/d0/src/dave_d0lib.c
    ${DAVE2D_DIR}/d0/src/dave_d0_mm_fixed_range_fixed_blkcnt.c
  )

  zephyr_library_sources_ifdef(
    CONFIG_WITH_MM_DYNAMIC
    ${DAVE2D_DIR}/d0/src/dave_d0lib.c
    ${DAVE2D_DIR}/d0/src/dave_d0_mm_dynamic.c
  )
endif()
