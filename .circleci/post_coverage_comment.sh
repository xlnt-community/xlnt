#!/bin/bash

set -eo pipefail

main ()
{
  if [ -z "${CIRCLE_PR_NUMBER}" ]; then
    if [[ $"${CIRCLE_PULL_REQUEST}" =~ https://github.com/xlnt-community/xlnt/pull/([0-9]+)$ ]]; then
      CIRCLE_PR_NUMBER=${BASH_REMATCH[1]}
    elif [[ $"${CIRCLE_BRANCH}" =~ pull/([0-9]+)(/.*)?$ ]]; then
      CIRCLE_PR_NUMBER=${BASH_REMATCH[1]}
    else
      echo "not associated with a PR"
      exit 0
    fi
  fi

  echo "Posting comment at PR $CIRCLE_PR_NUMBER"

  COMMIT_SHA=`git rev-parse HEAD`

  curl -L -X POST \
    -H "Authorization: Bearer ${PR_TOKEN}" \
    "https://api.github.com/repos/xlnt-community/xlnt/issues/${CIRCLE_PR_NUMBER}/comments" \
    -d "{\"body\":\"Coverage report is available at: [full](https://xlnt-community.github.io/xlnt-coverage/${COMMIT_SHA}/differential/index.html) | [review summary](https://xlnt-community.github.io/xlnt-coverage/${COMMIT_SHA}/review/index.html)\"}"
}

main

