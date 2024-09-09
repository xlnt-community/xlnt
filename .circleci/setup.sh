#!/bin/bash

set -eo pipefail

REPLACE_REGEX=""

isBuilt ()
{
    docker manifest inspect $1 > /dev/null
    return $?
}

setupDocker ()
{
    printf "\n * Setting up $1\n"

    DOCKERFILE="$1.Dockerfile"
    if [ -n "${DOCKERHUB_USERNAME}" ]; then
        REPOSITORY="${DOCKERHUB_USERNAME}/ci_$1"
    else
        REPOSITORY="xlntcommunity/ci_$1"
    fi
    LATEST_CHANGE_COMMIT=`git log -n 1 --pretty=format:%H -- "${DOCKERFILE}"`
    CURRENT_COMMIT=`git rev-parse HEAD`
    TAG=${LATEST_CHANGE_COMMIT}
    IMAGE=${REPOSITORY}:${TAG}

    if isBuilt "${IMAGE}"; then
        echo "${DOCKERFILE} unchanged: using already built image with tag ${TAG}"
    else
        echo "${DOCKERFILE} changed: building new docker image with tag ${TAG}"

        docker build -t ${IMAGE} --file "${DOCKERFILE}" .
        echo ${DOCKERHUB_PASSWORD} | docker login -u ${DOCKERHUB_USERNAME} --password-stdin
        docker push ${IMAGE}
    fi

    REPLACE_REGEX+="${REPLACE_REGEX} -e s#PLACEHOLDER_IMAGE($1)#${IMAGE}#"
}

main ()
{
    for filename in *.Dockerfile; do
        setupDocker $(basename "$filename" .Dockerfile)
    done

    sed ${REPLACE_REGEX} continue_config_in.yml > continue_config.yml
}

main

