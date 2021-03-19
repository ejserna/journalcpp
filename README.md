
# 1. Compile it
`g++ -std=c++17 -o journal client.cpp Content/Content.cpp Journal.cpp`

# 2. Run it
These will show you the available arguments you can pass.
`./journal`

# 3. Test it
The integration tests do the following:
Journal creation, entry creation, deletion and entry filtering and sorting.
`g++ -std=c++17 -o integrationtests Tests/IntegrationTests.cpp Content/Content.cpp Journal.cpp && ./integrationtests`
