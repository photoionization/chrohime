#!/usr/bin/env node

const cp = require('node:child_process')
const path = require('node:path')
const util = require('node:util')
const {pipeline} = require('node:stream/promises')

// Async exec.
const exec = util.promisify(cp.exec)

const rootDir = path.resolve(__dirname, '../..')
const buildDir = path.join(rootDir, 'website.build')
const meta = require('../meta.json')

// Load vendored modules.
module.paths.push('third_party/node_modules')
const fs = require('fs-extra')
const pug = require('pug')

async function main() {
  fs.emptyDirSync(buildDir)
  fs.copySync(path.join(rootDir, 'docs', 'website'), buildDir)

  const tags = await getTags()
  fs.writeFile(path.join(buildDir, 'index.html'),
               pug.renderFile(path.join(buildDir, 'site_index.pug'),
                              {latest: tags[0], versions: tags, ...meta}))

  await Promise.all(tags.map(extractDocs))
  await fs.copy(path.join(buildDir, 'docs', tags[0]),
                path.join(buildDir, 'docs', 'latest'))
}

async function extractDocs(tag) {
  const url = `https://github.com/${meta.repo}/releases/download/${tag}/chrohime_docs-${tag}.zip`
  const targetDir = path.join(buildDir, 'docs', tag)
  await fs.ensureDir(targetDir)
  const zipPath = path.join(targetDir, 'docs.zip')
  const stream = fs.createWriteStream(zipPath)
  const response = await fetch(url)
  await pipeline(response.body, stream)
  await exec('unzip docs.zip', {cwd: targetDir})
  await fs.remove(zipPath)
}

async function getTags() {
  const response = await fetch(`https://api.github.com/repos/${meta.repo}/releases`)
  const releases = await response.json()
  const tags = []
  for (const release of releases) {
    if (release.assets.find(a => a.name.includes('chrohime_docs-')))
      tags.push(release.tag_name)
  }
  return tags
}

main()
