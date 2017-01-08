#include "misc.h"

std::random_device _r;

std::default_random_engine RandomGlobals::GENERATOR = std::default_random_engine(_r());