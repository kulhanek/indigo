# ==============================================================================
# Indigo CMake File
# ==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(third_party/tinyxml/include)
INCLUDE_DIRECTORIES(common)
INCLUDE_DIRECTORIES(molecule)
INCLUDE_DIRECTORIES(layout)
INCLUDE_DIRECTORIES(graph)
INCLUDE_DIRECTORIES(api)

# objects in library -----------------------------------------------------------
SET(INDIGO_GENERAL_SRC
    # common -------------------------------------
#        common/oracle/ora_wrap.cpp
#        common/oracle/ora_error.cpp
#        common/oracle/ora_logger.cpp
        common/lzw/lzw_decoder.cpp
        common/lzw/lzw_encoder.cpp
        common/lzw/lzw_dictionary.cpp
#        common/unit_test/test_failure.cpp
#        common/unit_test/test_result_stderr.cpp
#        common/unit_test/test_result.cpp
#        common/unit_test/test.cpp
#        common/unit_test/test_registry.cpp
        common/math/vec3f.cpp
        common/math/transform3f.cpp
        common/math/vec2f.cpp
        common/math/plane3f.cpp
        common/math/lseg3f.cpp
        common/math/line3f.cpp
        common/math/best_fit.cpp
        common/math/matr3x3d.cpp

        common/base_cpp/bitinworker.cpp
        common/base_cpp/bitoutworker.cpp
        common/base_cpp/cancellation_handler.cpp
        common/base_cpp/chunk_storage.cpp
        common/base_cpp/common_exceptions_impl.cpp
        common/base_cpp/crc32.cpp
        common/base_cpp/d_bitset.cpp
        common/base_cpp/exception.cpp
        common/base_cpp/gray_codes.cpp
        common/base_cpp/io_base.cpp
        common/base_cpp/locale_guard.cpp
        common/base_cpp/os_sync_wrapper.cpp
        common/base_cpp/os_thread_wrapper.cpp
        common/base_cpp/output.cpp
        common/base_cpp/profiling.cpp
        common/base_cpp/properties_map.cpp
        common/base_cpp/scanner.cpp
        common/base_cpp/shared_ptr.cpp
        common/base_cpp/shmem_posix.cpp
        common/base_cpp/shmem_win32.cpp
        common/base_cpp/smart_output.cpp
        common/base_cpp/string_pool.cpp
        common/base_cpp/tlscont.cpp

        common/gzip/gzip_output.cpp
        common/gzip/gzip_scanner.cpp
    # tinyxml ------------------------------------
        third_party/tinyxml/src/tinyxml.cpp
        third_party/tinyxml/src/tinyxmlerror.cpp
        third_party/tinyxml/src/tinyxmlparser.cpp
        third_party/tinyxml/src/tinystr.cpp
    # molecule -----------------------------------
        molecule/src/base_molecule.cpp
        molecule/src/canonical_smiles_saver.cpp
        molecule/src/cmf_loader.cpp
        molecule/src/cmf_saver.cpp
        molecule/src/elements.cpp
        molecule/src/gross_formula.cpp
        molecule/src/haworth_projection_finder.cpp
        molecule/src/icm_loader.cpp
        molecule/src/icm_saver.cpp
#        molecule/src/inchi_parser.cpp
#        molecule/src/inchi_wrapper.cpp
        molecule/src/max_common_submolecule.cpp
        molecule/src/molecule_3d_constraints.cpp
        molecule/src/molecule_allene_stereo.cpp
        molecule/src/molecule_arom.cpp
        molecule/src/molecule_arom_match.cpp
        molecule/src/molecule_auto_loader.cpp
        molecule/src/molecule_automorphism_search.cpp
        molecule/src/molecule_cdx_loader.cpp
        molecule/src/molecule_cdxml_saver.cpp
        molecule/src/molecule_cdx_saver.cpp
        molecule/src/molecule_chain_fingerprints.cpp
        molecule/src/molecule_cis_trans.cpp
        molecule/src/molecule_cml_loader.cpp
        molecule/src/molecule_cml_saver.cpp
        molecule/src/molecule.cpp
        molecule/src/molecule_dearom.cpp
        molecule/src/molecule_electrons_localizer.cpp
        molecule/src/molecule_exact_matcher.cpp
        molecule/src/molecule_exact_substructure_matcher.cpp
        molecule/src/molecule_fingerprint.cpp
        molecule/src/molecule_inchi_component.cpp
        molecule/src/molecule_inchi.cpp
        molecule/src/molecule_inchi_layers.cpp
        molecule/src/molecule_inchi_utils.cpp
        molecule/src/molecule_ionize.cpp
        molecule/src/molecule_layered_molecules.cpp
        molecule/src/molecule_mass.cpp
        molecule/src/molecule_neighbourhood_counters.cpp
        molecule/src/molecule_pi_systems_matcher.cpp
        molecule/src/molecule_rgroups_composition.cpp
        molecule/src/molecule_rgroups.cpp
        molecule/src/molecule_savers.cpp
        molecule/src/molecule_scaffold_detection.cpp
        molecule/src/molecule_sgroups.cpp
        molecule/src/molecule_standardize.cpp
        molecule/src/molecule_standardize_options.cpp
        molecule/src/molecule_stereocenter_options.cpp
        molecule/src/molecule_stereocenters.cpp
        molecule/src/molecule_substructure_matcher.cpp
        molecule/src/molecule_tautomer_chain.cpp
#        molecule/src/molecule_tautomer_enumerator.cpp
        molecule/src/molecule_tautomer_match.cpp
        molecule/src/molecule_tautomer_matcher.cpp
        molecule/src/molecule_tautomer_substructure_matcher.cpp
        molecule/src/molecule_tautomer_superstructure.cpp
        molecule/src/molecule_tautomer_utils.cpp
        molecule/src/molecule_tgroups.cpp
        molecule/src/molfile_loader.cpp
        molecule/src/molfile_saver.cpp
        molecule/src/multiple_cdx_loader.cpp
        molecule/src/multiple_cml_loader.cpp
        molecule/src/query_molecule.cpp
        molecule/src/rdf_loader.cpp
        molecule/src/sdf_loader.cpp
        molecule/src/smiles_loader.cpp
        molecule/src/smiles_saver.cpp
    # reaction -----------------------------------
        reaction/src/base_reaction.cpp
        reaction/src/base_reaction_substructure_matcher.cpp
        reaction/src/canonical_rsmiles_saver.cpp
        reaction/src/crf_loader.cpp
        reaction/src/crf_saver.cpp
        reaction/src/icr_loader.cpp
        reaction/src/icr_saver.cpp
        reaction/src/query_reaction.cpp
        reaction/src/reaction_auto_loader.cpp
        reaction/src/reaction_automapper.cpp
        reaction/src/reaction_cdx_loader.cpp
        reaction/src/reaction_cdx_saver.cpp
        reaction/src/reaction_cml_loader.cpp
        reaction/src/reaction_cml_saver.cpp
        reaction/src/reaction.cpp
        reaction/src/reaction_enumerator_state.cpp
        reaction/src/reaction_exact_matcher.cpp
        reaction/src/reaction_fingerprint.cpp
        reaction/src/reaction_neighborhood_counters.cpp
        reaction/src/reaction_product_enumerator.cpp
        reaction/src/reaction_substructure_matcher.cpp
        reaction/src/reaction_transformation.cpp
        reaction/src/rsmiles_loader.cpp
        reaction/src/rsmiles_saver.cpp
        reaction/src/rxnfile_loader.cpp
        reaction/src/rxnfile_saver.cpp
    # layout -------------------------------------
        layout/src/attachment_layout.cpp
        layout/src/layout_pattern.cpp
        layout/src/layout_pattern_smart.cpp
        layout/src/metalayout.cpp
        layout/src/molecule_layout.cpp
        layout/src/molecule_layout_graph_assign.cpp
        layout/src/molecule_layout_graph_assign_smart.cpp
        layout/src/molecule_layout_graph_attach.cpp
        layout/src/molecule_layout_graph_border.cpp
        layout/src/molecule_layout_graph.cpp
        layout/src/molecule_layout_graph_cycle.cpp
        layout/src/molecule_layout_graph_geom.cpp
        layout/src/molecule_layout_graph_refine.cpp
        layout/src/molecule_layout_graph_smart.cpp
        layout/src/molecule_layout_macrocycle_lattice.cpp
        layout/src/molecule_layout_macrocycles.cpp
        layout/src/reaction_layout.cpp
        layout/src/refinement_state.cpp
#        layout/patmake/patmake.cpp
    # graph --------------------------------------
        graph/src/automorphism_search.cpp
        graph/src/aux_path_finder.cpp
        graph/src/biconnected_decomposer.cpp
        graph/src/cycle_basis.cpp
        graph/src/cycle_enumerator.cpp
        graph/src/dfs_walk.cpp
        graph/src/edge_rotation_matcher.cpp
        graph/src/edge_subgraph_enumerator.cpp
        graph/src/embedding_enumerator.cpp
        graph/src/embeddings_storage.cpp
        graph/src/filter.cpp
        graph/src/graph_affine_matcher.cpp
        graph/src/graph_constrained_bmatching_finder.cpp
        graph/src/graph.cpp
        graph/src/graph_decomposer.cpp
        graph/src/graph_fast_access.cpp
        graph/src/graph_iterators.cpp
        graph/src/graph_perfect_matching.cpp
        graph/src/graph_subchain_enumerator.cpp
        graph/src/graph_subtree_enumerator.cpp
        graph/src/max_common_subgraph.cpp
        graph/src/morgan_code.cpp
        graph/src/path_enumerator.cpp
        graph/src/scaffold_detection.cpp
        graph/src/shortest_path_finder.cpp
        graph/src/simple_cycle_basis.cpp
        graph/src/skew_symmetric_flow_finder.cpp
        graph/src/skew_symmetric_network.cpp
        graph/src/spanning_tree.cpp
        graph/src/subgraph_hash.cpp
    # indigo api ---------------------------------      
        api/src/indigo_abbreviations_core.cpp
        api/src/indigo_abbreviations_expand.cpp
        api/src/indigo_array.cpp
        api/src/indigo_basic.cpp
        api/src/indigo_composition.cpp
        api/src/indigo.cpp
        api/src/indigo_deconvolution.cpp
        api/src/indigo_fingerprints.cpp
        api/src/indigo_io.cpp
        api/src/indigo_layout.cpp
        api/src/indigo_loaders.cpp
        api/src/indigo_mapping.cpp
        api/src/indigo_match.cpp
        api/src/indigo_misc.cpp
        api/src/indigo_molecule.cpp
        api/src/indigo_object.cpp
        api/src/indigo_options.cpp
        api/src/indigo_product_enumerator.cpp
        api/src/indigo_properties.cpp
        api/src/indigo_reaction.cpp
        api/src/indigo_savers.cpp
        api/src/indigo_scaffold.cpp
        api/src/indigo_stereo.cpp
        api/src/indigo_tautomer_enumerator.cpp
        api/src/option_manager.cpp
        api/src/indigo_macros.c
)

SET(INDIGO_UNIX_SRC
        common/base_c/os_sync_posix.c
        common/base_c/os_tls_posix.c
        common/base_c/nano_posix.c
        common/base_c/os_dir_posix.c
        common/base_c/os_thread_posix.c
        common/base_c/bitarray.c
)

SET(INDIGO_WIN32_SRC
        common/base_c/os_sync_win32.c
        common/base_c/os_thread_win32.c
        common/base_c/nano_win.c
        common/base_c/os_tls_win32.c
        common/base_c/os_dir_win32.c
        common/base_c/bitarray.c
)

IF(WIN32)
    ADD_DEFINITIONS(-D_WIN32)
    SET(INDIGO_SRC ${INDIGO_WIN32_SRC} ${INDIGO_GENERAL_SRC})
ELSE(WIN32)
    SET(INDIGO_SRC ${INDIGO_UNIX_SRC} ${INDIGO_GENERAL_SRC})
ENDIF(WIN32)

# create static library --------------------------------------------------------
IF(LIBS_STATIC)
    ADD_LIBRARY(indigo_static STATIC ${INDIGO_SRC})
    SET_TARGET_PROPERTIES(indigo_static PROPERTIES
                            OUTPUT_NAME indigo
                            CLEAN_DIRECT_OUTPUT 1)
ENDIF(LIBS_STATIC)

# create shared library --------------------------------------------------------
IF(LIBS_SHARED)
    ADD_LIBRARY(indigo_shared SHARED ${INDIGO_SRC})
    TARGET_LINK_LIBRARIES(indigo_shared ${ZLIB_LIB})
    SET_TARGET_PROPERTIES(indigo_shared PROPERTIES
                            OUTPUT_NAME indigo
                            CLEAN_DIRECT_OUTPUT 1
                            VERSION ${INDIGO_VERSION})
    INSTALL(TARGETS indigo_shared
            DESTINATION lib)
ENDIF(LIBS_SHARED)
