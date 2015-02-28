#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QMutexLocker>
#include <QDebug>

#include "spike.h"
#include "gitrepository.h"
#include "gitcommit.h"

QMutex GitRepository::gitMutex;

GitRepository::GitRepository(const QString& repo) : repodirba_(repo.toUtf8()), repodir_(repodirba_.data()) {
  git_threads_init();
  QMutexLocker l(&gitMutex);
  int error = git_repository_open(&repo_, repodir_);
  if(error <0) {
    l.unlock();
    qDebug() << "creating repository";
    createRepository(); 
    openRepository();
  }
}

GitRepository::~GitRepository() {
  git_repository_free(repo_); 
  git_threads_shutdown();
}

bool GitRepository::commitIndex(GitIndex& index) {
  return commitIndex(index.index_);
}

bool GitRepository::commitIndex(git_index* index) {
  //  write git index
  git_signature *sig = nullptr;
  git_oid tree_id;
  git_oid commit_id;
  git_tree *tree = nullptr;

  int error = git_signature_now(&sig, "AutoGit", "auto@localhost");
  if (!gitErrorCheck(error)) return false;

  git_commit * commit = nullptr; // parent
  git_oid oid_parent_commit;  // the SHA1 for last commit

  error = git_reference_name_to_id( &oid_parent_commit, repo_, "HEAD" );
  if (!gitErrorCheck(error)) return false;

  error = git_commit_lookup( &commit, repo_, &oid_parent_commit );
  if (!gitErrorCheck(error)) return false;

  error = git_index_write(index);
  if (!gitErrorCheck(error)) return false;

  error = git_index_write_tree(&tree_id, index);
  if (!gitErrorCheck(error)) return false;

  error = git_tree_lookup(&tree, repo_, &tree_id);
  if (!gitErrorCheck(error)) return false;

  const git_commit* parentarray[] = {commit};
  const QString message("Auto commit");
  QByteArray commitmessageba = message.toUtf8();
  const char *commitmessageCString = commitmessageba.data();
  error = git_commit_create(&commit_id, repo_, "HEAD", sig, sig, nullptr, commitmessageCString, tree, 1, parentarray);
  if (!gitErrorCheck(error)) return false;
  
  git_signature_free(sig);
  git_tree_free(tree);
  return true;
}

const QList<GitCommitPtr> GitRepository::walkHistory(GitRepositoryPtr repo) const {
  git_revwalk *walk;
  git_revwalk_new(&walk, repo_);
  git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL | GIT_SORT_TIME);
  git_revwalk_push_head(walk);
  git_revwalk_hide_glob(walk, "tags/*");
  git_oid oid;
  
  QList<GitCommitPtr> l;
  while (git_revwalk_next(&oid, walk) == 0) {
    git_commit* c;
    git_commit_lookup(&c, repo_, &oid);

    GitCommitPtr cp(new GitCommit(c, repo));
    l << cp;
  }
  git_revwalk_free(walk);
  return l;
}

bool GitRepository::openRepository() {
  QMutexLocker l(&gitMutex);
  int error = git_repository_open(&repo_, repodir_);
  return gitErrorCheck(error);
}

void GitRepository::createRepository() {
  QMutexLocker l(&gitMutex);
  int error = git_repository_init(&repo_, repodir_, false);
  if(!gitErrorCheck(error)) return;
  
  git_signature *sig = nullptr;
  git_index *index = nullptr;
  git_oid tree_id;
  git_oid commit_id;
  git_tree *tree = nullptr;

  // no error handling at the moment
  git_signature_now(&sig, "AutoGit", "auto@localhost");
  git_repository_index(&index, repo_);
  git_index_write_tree(&tree_id, index);
  git_tree_lookup(&tree, repo_, &tree_id);
  git_commit_create_v(&commit_id, repo_, "HEAD", sig, sig, nullptr, "Initial commit", tree, 0);

  git_signature_free(sig);
  git_index_free(index);
  git_tree_free(tree);
}

GitIndex::GitIndex(GitRepositoryPtr r): repo_(r) {
  int error = git_repository_index(&index_, repo_->repo_);
  if (error <0) index_ = nullptr;
}


void GitIndex::add(const SpikePtr& s) {
  QMutexLocker l(&(repo_->gitMutex));
  if (index_!=nullptr) {
    const QString dirname = s->dirName() + "/*";
    QByteArray ba = dirname.toUtf8();
    char* baCString = ba.data();
    git_strarray arr = { &baCString, 1};
    
    int error = git_index_add_all(index_, &arr, 0,nullptr,nullptr);
    if (error <0) { 
      qDebug() << "error with adding spike";
      gitErrorHandling();
    }
  } else qDebug() << "Index is null";
}

void GitIndex::add(const SpikesTreeModel& stm) {
  Q_UNUSED(stm);
  QMutexLocker l(&(repo_->gitMutex));
  if (index_!=nullptr) {
    const QString path = "spikestree.xml";
    const QByteArray ba = path.toUtf8();
    const char* baCString = ba.data();
    int error = git_index_add_bypath(index_, baCString);
    if (error < 0) {
     qDebug() << "error with adding tree";
     gitErrorHandling();
    }
  } else qDebug() << "Index is null";
}

void GitIndex::removeSpike(const SpikePtr& s) {
  if (index_!=nullptr) {
    const QString dir = s->dirName();
    const QByteArray dirba = dir.toUtf8();
    const char* dirCString = dirba.data();
    int error = git_index_remove_directory(index_, dirCString, 0);
    if (!gitErrorCheck(error)) return;
  }
}

void GitIndex::removeFiles(const QStringList& l) {
  for(const QString& f: l)
    removeFile(f);
}

void GitIndex::removeFile(const QString& f) {
  qDebug() << "remove" << f;
  if (index_!=nullptr) {
    const QByteArray ba = f.toUtf8();
    const char* baCString = ba.data();
    int error = git_index_remove_bypath(index_, baCString);
    if (!gitErrorCheck(error)) return;
  }
}

