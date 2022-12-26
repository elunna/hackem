## Things Peeps Can Do To Contribute To Hack'EM

#### Improve the contribution guidelines (this file!)
* Read this and related docs: https://docs.github.com/en/communities/setting-up-your-project-for-healthy-contributions/setting-guidelines-for-repository-contributors
* Ask us questions
* Update this file (CONTRIBUTING.md) with your findings

#### Improve other documents
* Generally, editing txt/md files can be done here on github. After editing, scroll down to write a commit title and commit description
* README.md
* doc/changelog.md
  * Add things we've missed
  * Compress things, especially when the same thing has been changed multiple times - it can be condensed down to just say how the thing *currently* works, instead of containing a detailed history of gameplay that no longer exists 

#### Creating a PR
* if you haven't done so yet, "Fork" this repo on github ![image](https://user-images.githubusercontent.com/62170514/134273835-297c7819-a628-43a3-96a8-04f2734a6534.png)
* then clone *your* Hack'EM repo onto your computer. eg: `git clone https://github.com/elunna/hackem.git'
* move to a new branch: `git checkout -b FourNewGrenades`
* make your changes
  * if you finish up a change, like adding a new item, "commit" that, then make a separate commit when you do something different, like changing an existing item.
  * in other words, try to keep commits "atomic", here's why that will make your life easier: https://www.freshconsulting.com/insights/blog/atomic-commits/
* include succinct commit titles like "Add four new grenades" or "Remove lichen" or "Increase damage for skilled+ weapon use"
* push those changes to your repo: `git push --set-upstream upstream FourNewGrenades`
* open a Pull Request here, on elunna/hackem
* alternate set of instructions for creating a PR: https://docs.github.com/en/articles/creating-a-pull-request

#### Create Bug Reports
* https://github.com/elunna/hackem/issues
* Screenshots can be helpful

#### Update the Hack'EM section on the NetHack wiki
* This was a good project for anyone!
* Check grammer and spelling.
* Where needed, make revisions to clarify the content. 
* https://nethackwiki.com/wiki/Hack%27EM

Credit and Thanks to SpliceHack for setting up this document!