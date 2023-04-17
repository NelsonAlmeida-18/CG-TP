import math
import sys
import os

distanceScale = 1/10**6
planetScale = 1/1000


def solarSystemGenerator():
    scale=1
    filename="solarSystem.xml"
    if (sys.argv[1]):
        scale=float(sys.argv[1])
    if (sys.argv[2]):
        filename=sys.argv[2]

        
    xmlHeader = f"""
    <world>
        <window width="512" height="512" />
        <camera>
            <position x="250" y="10" z="250" />
            <lookAt x="0" y="0" z="0" />
            <up x="0" y="1" z="0" />
            <projection fov="60" near="1" far="1000" />
        </camera>
        <lights>
            <light type="point" posX="0" posY="10" posZ="0" />
            <light type="directional" dirX="1" dirY="1" dirZ="1" />
            <light type="spotlight" posX="0" posY="10" posZ="0" dirX="1" dirY="1" dirZ="1" cutoff="45" />
        </lights>
        <group>
    """

    finalXML = "    </group>\n</world>"

    astros = {"sun":1392*10**3, "mercury":4878, "venus":12104, "earth":12756, "mars":6787, "jupiter":142796, "saturn":120660, "uranus":51118, "neptune": 24622}
    distances = {"sun":0, "mercury":58*10**6 + 1392*10**6, "venus":108.2*10**6 + 1392*10**6, "earth":150*10**6 + 1392*10**6, "mars":227.9*10**6 + 1392*10**6, "jupiter":778.3*10**6 + 1392*10**6, "saturn":1427*10**6 + 1392*10**6, "uranus":2871*10**6 + 1392*10**6, "neptune":4497.1*10**6 + 1392*10**6}
    for x in astros:
        if x == "saturn":
            generatePlanet(x, scale*planetScale*astros[x])
            generateRing(planetScale*scale*astros["earth"]*21, planetScale*scale*astros["earth"]*16.5)
            xmlHeader+=generateTransformations(x, scale*distanceScale*distances[x], 1)
        else:
            generatePlanet(x, scale*planetScale*astros[x])
            xmlHeader+=generateTransformations(x, scale*distanceScale*distances[x], 0)

    xmlHeader+=finalXML
    file = open(filename, "w")
    file.write(xmlHeader)
    file.close()
    print("Solar system generated!")


def generateTransformations(name,distance,torus):
    template = f"""
    <group>
        <transform>
            <translate x="{distance}" y="0" z="0" />
            <scale x="1" y="1" z="1" />
        </transform>
        <models>
            <model file="solarSystem/{name}.3d" />
        </models>
    </group>
    """

    if torus:
        template += f"""
        <group>
            <transform>
                <translate x="{distance}" y="0" z="0" />
                <scale x="1" y="1" z="1" />
                <rotate angle="60" x="1" y="0" z="0" />
                <rotate angle="40" x="0" y="0" z="1" />
            </transform>
            <models>
                <model file="solarSystem/torus.3d" />
            </models>
        </group>
        """

    return template


def generateRing(outer_r, inner_r):
    #step 1 find if generator in path
    if "generator" in os.listdir():
        if "solarSystem" not in os.listdir("../3d/"):
            os.system("mkdir ../3d/solarSystem")
        command = "./generator torus 20 5 0.5 30 30 ../3d/solarSystem/torus.3d"
        os.system(command)
    else:
        if "generator.cpp" in os.listdir():
            os.system("g++ generator.cpp -o generator")
            generateRing(outer_r, inner_r)
        else:
            print("Could not find a generator")


def generatePlanet(planetName, dimensions):
    #step 1 find if generator in path
    if "generator" in os.listdir():
        if "solarSystem" not in os.listdir("../3d/"):
            os.system("mkdir ../3d/solarSystem")
        command = f"./generator sphere {dimensions} {30} {30} ../3d/solarSystem/{planetName}.3d"
        os.system(command)
    else:
        if "generator.cpp" in os.listdir():
            os.system("g++ generator.cpp -o generator")
            generatePlanet(planetName, dimensions)
        else:
            print("Could not find a generator")

solarSystemGenerator()