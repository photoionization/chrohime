#!/usr/bin/env node

const path = require('path')

// Load vendored modules.
module.paths.push(path.resolve(__dirname, '../third_party/node_modules'))
const fs = require('fs-extra')
const pug = require('pug')

// Parameters.
const apisJsonPath = process.argv[2]
const version = process.argv[3]
const indexPug = process.argv[4]
const guidePug = process.argv[5]
const apiPug = process.argv[6]
const outDir = process.argv[7]
const guideFiles = process.argv.slice(8)

const lang = 'c'
const meta = require('./meta.json')

function main() {
  const langDir = path.join(outDir, lang)
  fs.emptyDirSync(langDir)

  const parse = getMarkdownParser(lang)
  const pugOptions = {
    ...meta,
    lang,
    version,
    markdown: parse,
    imarkdown: (text) => inlineMarkdown(parse, text),
    firstline: findFirstLine,
    filters: { 'css-minimize': cssMinimize },
  }

  // Load parsed APIs.
  const apis = fs.readJsonSync(apisJsonPath)

  // Find all guides.
  const guides = []
  for (const file of guideFiles) {
    if (!file.endsWith('.md')) continue
    const guide = parseMarkdown(parse, fs.readFileSync(file).toString())
    guide.id = path.basename(file, '.md')
    guides.push(guide)
  }
  guides.sort((a, b) => b.priority - a.priority)

  // Generate the index page.
  const html = pug.renderFile(indexPug, {
    page: 'index',
    doc: { name: 'Docs' },
    apis,
    guides,
    ...pugOptions,
  })
  fs.writeFileSync(path.join(langDir, 'index.html'), html)

  // Generate pages for guides.
  const guideDir = path.join(langDir, 'guide')
  fs.ensureDirSync(guideDir)
  for (let guide of guides) {
    const html = pug.renderFile(guidePug, {
      page: 'guide',
      doc: guide,
      ...pugOptions,
    })
    fs.writeFileSync(path.join(guideDir, `${guide.id}.html`), html)
  }

  // Generate pages for APIs.
  const apiDir = path.join(langDir, 'api')
  fs.ensureDirSync(apiDir)
  for (let api of apis) {
    const html = pug.renderFile(apiPug, {
      name: api.name,
      page: 'api',
      doc: api,
      ...pugOptions,
    })
    fs.writeFileSync(path.join(apiDir, `${api.id}.html`), html)
  }
}

function getMarkdownParser(lang) {
  const marked = require('marked')
  const hljs = require('highlight')

  // A renderer that highlights code.
  const renderer = new marked.Renderer()
  renderer.code = (code, language) => {
    if (language)
      code = hljs.highlight(code, {language, ignoreIllegals: true}).value;
    return '<pre>' + code + '</pre>'
  }

  // Automatically convert inline code.
  renderer.codespan = (code) => {
    const match = code.match(/^&lt;\!(.*)&gt;(.*)$/)
    if (match)
      code = parseInlineCode(renderer.lang, match[1], match[2])
    return code.startsWith('<') ? code : `<code>${code}</code>`
  }

  marked.setOptions({renderer})
  return (text) => marked.parse(text)
}

// Read markdown and parse yaml header if there is one.
function parseMarkdown(parse, content) {
  let lines = content.split('\n')
  let result = {}
  if (lines[0] == '---') {
    for (let i = 1; i < lines.length; ++i) {
      if (lines[i] == '---') {
        lines = lines.slice(i + 1)
        break
      }
      const [key, value] = lines[i].split(':')
      result[key.trim()] = value.trim()
    }
  }
  if (result.priority === undefined)
    result.priority = 0
  result.content = parse(lines.join('\n'))
  result.name = result.content.substring(result.content.indexOf('>') + 1,
                                         result.content.indexOf('</h1>'))
  return result
}

// Strip p tag around markdown result.
function inlineMarkdown(parse, str) {
  let markdown = parse(str)
  return markdown.substr(3, markdown.length - 8)
}

// Find the first line in a markdown text.
function findFirstLine(text) {
  let result = ''
  for (const line of text.split('\n')) {
    if (line.trim().length == 0)
      break;
    result += line
  }
  return result
}

// A simple CSS minimize function.
function cssMinimize(str) {
  let lines = str.split('\n')
  return lines.map((line) => line.trim()).join('')
}

main()
