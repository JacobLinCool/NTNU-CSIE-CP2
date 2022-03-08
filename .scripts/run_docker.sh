# Run Container
docker run -dit -v $PWD/source:/usr/workspace -w /usr/workspace --name cp-homework-container cp-homework

# Enter Interactive Shell
docker exec -it cp-homework-container bash

# Stop Container
docker stop cp-homework-container

# Remove Container
docker rm cp-homework-container
