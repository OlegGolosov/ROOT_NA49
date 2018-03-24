GET_TARGET_PROPERTY(T49DST_LIBRARY T49DST LOCATION)

set(T49ANA_SOURCES src/T49Container.C src/T49Cut.C src/T49Cut2Track.C
  src/T49CutEvent.C src/T49CutPid.C src/T49CutTrack.C
  src/T49CutV0.C src/T49Dedx.C src/T49DedxFunction.C
  src/T49DedxInfo.C src/T49HbtCoulomb.C src/T49HbtFitBase.C
  src/T49HbtFitDivers.C src/T49HbtMomentumResolutionCalculation.C
  src/T49HbtMomentumResolutionCorrection.C src/T49Histogram.C
  src/T49Histogram1D.C src/T49Histogram2D.C src/T49Histogram3D.C
  src/T49InMuDST.C src/T49Index.C src/T49Mixer.C
  src/T49Mixing.C src/T49MultiMixer.C src/T49OnePeak.C src/T49Padtrans.C
  src/T49Prob.C src/T49Proj1.C src/T49Proj2.C src/T49SumGaus.C src/T49Tool.C
  src/T49Trkstep.C src/T49XiFinder.C src/T49dEdxFunctions.C)
set(T49ANA_public_HEADERS inc/T49Container.h inc/T49Cut.h
  inc/T49Cut2Track.h inc/T49CutEvent.h inc/T49CutPid.h
  inc/T49CutTrack.h inc/T49CutV0.h inc/T49Dedx.h
  inc/T49DedxFunction.h inc/T49DedxInfo.h inc/T49HbtCoulomb.h
  inc/T49HbtFitBase.h inc/T49HbtFitDivers.h
  inc/T49HbtMomentumResolutionCalculation.h
  inc/T49HbtMomentumResolutionCorrection.h inc/T49Histogram.h
  inc/T49Histogram1D.h inc/T49Histogram2D.h inc/T49Histogram3D.h
  inc/T49InMuDST.h inc/T49Index.h inc/T49Mixer.h
  inc/T49Mixing.h inc/T49MultiMixer.h inc/T49OnePeak.h inc/T49Padtrans.h
  inc/T49Prob.h inc/T49Proj1.h inc/T49Proj2.h inc/T49SumGaus.h inc/T49Tool.h
  inc/T49Trkstep.h inc/T49XiFinder.h inc/T49dEdxFunctions.h)
set(T49ANA_HEADERS ${T49ANA_public_HEADERS})

ROOT_GENERATE_DICTIONARY(T49ANA
  LINKDEF inc/T49ANALinkDef.h
  INFILES ${T49ANA_HEADERS}
  INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${T49DST_INCLUDE_DIR} ${ROOTDS_INCLUDE_DIR}
)
LIST(APPEND T49ANA_SOURCES ${T49ANA_DICT_SOURCE})
LIST(APPEND T49ANA_HEADERS ${T49ANA_DICT_HEADER})

LIST(APPEND T49ANA_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${T49ANA_INCLUDE_DIR}
  ${T49DST_INCLUDE_DIR})
if(NOT DISABLE_DSPACK)
  LIST(APPEND T49ANA_INCLUDE_DIRECTORIES
    ${ROOTDS_INCLUDE_DIR})
endif(NOT DISABLE_DSPACK)
INCLUDE_DIRECTORIES(${T49ANA_INCLUDE_DIRECTORIES})

add_library(T49ANA SHARED ${T49ANA_SOURCES} ${T49ANA_HEADERS})
list(APPEND T49ANA_LINK_LIBRARIES
  ${ROOT_LIBS} ${ROOT_Minuit_LIBRARY}
  T49DST
)
if(NOT DISABLE_PADTRANS)
  list(APPEND T49ANA_LINK_LIBRARIES ${NA49_padtrans_LIBRARY})
endif(NOT DISABLE_PADTRANS)
if(NOT DISABLE_TRKSTEP)
  list(APPEND T49ANA_LINK_LIBRARIES ${NA49_trkstep_LIBRARY})
endif(NOT DISABLE_TRKSTEP)
target_link_libraries(T49ANA ${T49ANA_LINK_LIBRARIES})

INSTALL(TARGETS T49ANA DESTINATION lib)
install(FILES ${T49ANA_public_HEADERS} DESTINATION include/T49/T49ANA)
