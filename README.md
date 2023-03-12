# ADAM (Asset Drone Assisted Monitoring)
## NXP HoverGames3: Land, Sky, Food Supply
As part of the NXP contest, this repository holds all of the files used to make the ADAM project.

*Insert logo here*

## The challenge and the motivation for taking part in this contest.
According to a report by SIAP (Servicio de Información Agroalimentaria y Pesquera/ Agri-Food and Fisheries Information Service) [1], the total number of cattle in Mexico in 2021 was 35,998,885. This industry is a vital part of the country's economy, providing food and raw materials of animal origin.

[^1]: https://www.gob.mx/siap/documentos/poblacion-ganadera-136762?idiom=es

In conversations with local landowners whose primary source of income is the agricultural industry, specifically in cattle production, a recurring concern is the frequent loss of cows due to poor infrastructure, unauthorized access by individuals engaging in outdoor sports, and other factors.  

Thus, the need of constant monitoring of stock is needed, however, human monitoring thousands of miles of land is exhaustive and nearly impossible to do.  
NXP HoverGames3 opens an oportunity to explore this issue and to also give a solution for it.

*Insert representative image of the issue*

## Our proposed project and solution to address the issue at hand.
With the help of the [HoverGames drone kit](https://www.nxp.com/design/designs/nxp-hovergames-drone-kit-including-rddrone-fmuk66-and-peripherals:KIT-HGDRONEK66) covering large land surface is possible.  
### The monitoring part
Each cattle will be equipped with an RX LoRa Low Power device, which whill send periodically the GPS data in order to know its specific location. This device should not need constant charging and will deply useful information about its state.

The drone will include an RX and TX transmitter, collecting all the data from each asset and deploying it to the closest ground station, sending the location and status of the asset.
