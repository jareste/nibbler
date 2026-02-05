# Nibbler - Devlog - 13

## Table of Contents
1. [Day Seventeen Plan](#131-day-seventeen-plan)
2. [Testing, Testng, One, Two, Three](#132-testing-testng-one-two-three)

<br>
<br>

## 13.1 Day Seventeen Plan
Today is all about automated tests. Well, I hope it is going to be *more* than about those, but we'll start from there, for which we'll need to set up:
- github actions
- makefile build pipeline and test-testing files to set up a `gtest` (Google) environment
- write a bunch of tests

If after all of this there's more time to today's journey, I'll get into... **checks notes*... I don't know, building the foundations for a multisnake mode or something, don't ask me, what do i know.

<br>
<br>

## 13.2 Testing, Testng, One, Two, Three
I have some experience setting up github actions with automated test pipelines based on different tools. For example, I've worked with `xUnit` in `C#`/`dotnet` environments and `criterion` for `C` programs, and after some research I decided to take the opportunity to learn about `gtest` tied to `C++`. All of these tools, as I understand them at least, give me the perfect environment to both make **assertion based tests** that can be automated a different points during development and integration (aka, we're getting into `CI/CL` from this point on), and the objective today is to **build a pipeline that auto-tests any attempt to push or merge along the project's tree**. So let's get into it!

