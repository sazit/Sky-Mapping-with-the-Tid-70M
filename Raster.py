%matplotlib nbagg
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from astropy.coordinates import Angle
from astropy import units as u
from astropy.coordinates import SkyCoord
from mpl_toolkits.mplot3d import Axes3D
from __future__ import print_function
from astropy.visualization import astropy_mpl_style
from astropy.time import Time
from astropy.coordinates import SkyCoord, EarthLocation, AltAz
from astropy.coordinates import get_moon
from astropy.coordinates import Angle
from mpl_toolkits.basemap import Basemap
from astropy.coordinates import SkyOffsetFrame, ICRS

path = '/Users/sajitgurubacharya/Desktop/OTF/OTF.csv'
f = open(path, 'r')
data = f.read()
rows = data.split('\n')
finalData=(rows[4:])

radianChange=(0.015*np.pi/180)*u.rad
AZ=finalData[::3]
ALT=finalData[1::3]
TIME=finalData[2::3]
AZrad=[None]*len(AZ)
ALTrad=[None]*len(AZ)
newBeam1=[None]*len(AZ)
newBeam2=[None]*len(AZ)
AZrad1=[None]*len(AZ)
AZrad2=[None]*len(AZ)

for i in range(len(AZ)):
    AZrad[i]=float(AZ[i])*u.rad
    ALTrad[i]=float(ALT[i])*u.rad

for i in range(len(AZ)):
    AZrad1[i]=AZrad[i]+radianChange
    AZrad2[i]=AZrad[i]-radianChange
    
earthLat=-35.402
earthLon=148.981

localTime=Time(TIME)
latLon=EarthLocation(lat=earthLat,lon=earthLon)

degreeChange=0.015
radianChange=degreeChange*np.pi/180*u.rad

newBeam1= SkyCoord(alt = ALTrad , az =AZrad1, obstime = localTime, frame = 'altaz', location = latLon)
newBeam2= SkyCoord(alt = ALTrad , az =AZrad2, obstime = localTime, frame = 'altaz', location = latLon)

newBeam1RADEC=SkyCoord(ra=newBeam1.icrs.ra, dec=newBeam1.icrs.dec,frame='icrs')
newBeam2RADEC=SkyCoord(ra=newBeam2.icrs.ra, dec=newBeam2.icrs.dec,frame='icrs')
c=newBeam1RADEC.separation(newBeam2RADEC)
c2=newBeam1.separation(newBeam2)

from matplotlib.colors import LogNorm
beamRA=(newBeam1RADEC.ra,newBeam2RADEC.ra)
beamRA=np.append(beamRA[0].radian,beamRA[1].radian)

beamDEC=(newBeam1RADEC.dec,newBeam2RADEC.dec)
beamDEC=np.append(beamDEC[0].radian,beamDEC[1].radian)

plt.figure(figsize=(5,5))
plt.title("Beam 2")
plt.hist2d(newBeam2RADEC.ra, newBeam2RADEC.dec, bins=144, range=None, normed=None, weights=None,cmap='inferno', hold=None)
plt.colorbar()
plt.show()


plt.figure(figsize=(5,5))
plt.title("Beam 1")
plt.hist2d(newBeam1RADEC.ra, newBeam1RADEC.dec, bins=144, range=None, normed=None, weights=None,cmap='inferno', hold=None)
plt.colorbar()
plt.show()

plt.figure(figsize=(5,5))
plt.title("Final Raster")
plt.hist2d(beamRA*(180/np.pi), beamDEC*(180/np.pi), bins=144, range=None, normed=None, weights=None,cmap='inferno', hold=None)
plt.colorbar()
plt.show()




