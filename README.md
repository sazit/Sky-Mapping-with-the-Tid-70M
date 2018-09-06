# SkyMapping simulator for the Tid70M Radio Telescope

Simulation of Sky Mapping using the Tidbinbilla Radio Telescope

This project aims to simulate sky positions the Tid-70m would be pointing at given an Earth location and time when conducting On-the-fly (OTF) Mapping. The required scan position can be input in RA&Dec. The scan area is currently set to 1 degree by 1 degree. The code outputs coordinates in AltAz for the telescope. It can be used to determine whether a certain part of the sky is scannable at a given time. It also removes reduant scan points^. It also takes into account the sampling rate and thus the time taken throughout the scanning process.


^The Tid-70m has two receivers set 30 millidegrees apart in azimuth. Thus a normal scan would result in many re-scans of the same point in the sky. This depends on the distance each next step in a scan stripes takes and the distance between stripes as well.

Report can be found here:
https://zenodo.org/record/1185437
