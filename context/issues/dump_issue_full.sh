gh issue view $1 \
  --repo kshakirov/modern-cpp-inmemory-db \
  --json number,title,state,author,createdAt,updatedAt,url,body,comments \
  > issue-$1.json
