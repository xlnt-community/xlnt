#!/bin/bash

set -eo pipefail

PR_NUMBER_OVERIDE=$1

post_coverage_comment ()
{
  PR_NUMBER=$1
  COMMIT_SHA=$2

  echo "Posting comment at PR $PR_NUMBER"

  curl -L -X POST \
    -H "Authorization: Bearer ${PR_TOKEN}" \
    "https://api.github.com/repos/xlnt-community/xlnt/issues/${PR_NUMBER}/comments" \
    -d "{\"body\":\"Coverage report is available at: [full](https://xlnt-community.github.io/xlnt-coverage/${COMMIT_SHA}/differential/index.html) | [review summary](https://xlnt-community.github.io/xlnt-coverage/${COMMIT_SHA}/review/index.html)\"}"
}

main ()
{
  COMMIT_SHA=`git rev-parse HEAD`

  if [ -n "${PR_NUMBER_OVERIDE}" ]; then
    post_coverage_comment ${PR_NUMBER_OVERIDE} ${COMMIT_SHA}
  elif [ -n "${CIRCLE_PR_NUMBER}" ]; then
    post_coverage_comment ${CIRCLE_PR_NUMBER} ${COMMIT_SHA}
  elif [[ $"${CIRCLE_PULL_REQUEST}" =~ https://github.com/xlnt-community/xlnt/pull/([0-9]+)$ ]]; then
    post_coverage_comment ${BASH_REMATCH[1]} ${COMMIT_SHA}
  elif [[ $"${CIRCLE_BRANCH}" =~ pull/([0-9]+)(/.*)?$ ]]; then
    post_coverage_comment ${BASH_REMATCH[1]} ${COMMIT_SHA}
  else
    echo "not associated with a PR"
  fi
}

main

