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
* https://github.com/NullCGT/SpliceHack/issues
* in the future, a good Issue template would be helpful, anyone can write it!

#### Take "static" level files and use Lua to make them dynamic
* This was a good project for one of the newer SpliceHack devs. Check out dat/castle.lua and in particular look at sections of the level that have a percent chance() of being in play. this shuffles a little bit of variety into what was otherwise, previously, a classic, static "DES" file.
* This is the documentation for the Lua format: 
* https://github.com/NullCGT/SpliceHack/blob/Spl-R-1.2.0/doc/lua.adoc
* This describes the way DES files used to be done: https://nethackwiki.com/wiki/Des-file_format
* Some of the things that can be done in Lua were also possible with DES files, and the documentation in the DES page is a bit more detailed, so it's worth checking if you get stuck!
* Look in the dat/ folder for other Lua files that describe levels. If the map is one static thing without any variety, maybe work in a random chance for some grass, or a random chance to change the shape of a terrain feature.

--hackem: Credit and Thanks to SpliceHack for setting up this document!