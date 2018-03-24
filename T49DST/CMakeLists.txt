set(T49DST_SOURCES src/T49AbsClasses.C src/T49BeamRoot.C src/T49EventMCRoot.C
  src/T49EventRoot.C src/T49MonitorTPCRoot.C src/T49ParticleIdRoot.C
  src/T49ParticleMCRoot.C src/T49ParticleRoot.C src/T49RingRoot.C src/T49Run.C
  src/T49VertexMCRoot.C src/T49VertexRoot.C src/T49VetoRoot.C)
set(T49DST_public_HEADERS inc/PhysConst.h inc/T49Beam.h inc/T49BeamRoot.h
  inc/T49Event.h inc/T49EventMCRoot.h inc/T49EventRoot.h inc/T49Geant.h
  inc/T49MonitorTPC.h inc/T49MonitorTPCRoot.h inc/T49Particle.h
  inc/T49ParticleIdRoot.h inc/T49ParticleMCRoot.h inc/T49ParticleRoot.h
  inc/T49Ring.h inc/T49RingRoot.h inc/T49Run.h inc/T49Vertex.h
  inc/T49VertexMCRoot.h inc/T49VertexRoot.h inc/T49Veto.h inc/T49VetoRoot.h)
set(T49DST_HEADERS ${T49DST_public_HEADERS})

ROOT_GENERATE_DICTIONARY(T49DST
  LINKDEF inc/T49DSTLinkDef.h
  INFILES ${T49DST_HEADERS}
  INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
)
LIST(APPEND T49DST_SOURCES ${T49DST_DICT_SOURCE})
LIST(APPEND T49DST_HEADERS ${T49DST_DICT_HEADER})

LIST(APPEND T49DST_INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${T49DST_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${T49DST_INCLUDE_DIRECTORIES})

add_library(T49DST SHARED ${T49DST_SOURCES} ${T49DST_HEADERS})
TARGET_LINK_LIBRARIES(T49DST
  ${ROOT_LIBS}
)

INSTALL(TARGETS T49DST DESTINATION lib)
install(FILES ${T49DST_public_HEADERS} DESTINATION include/T49/T49DST)
