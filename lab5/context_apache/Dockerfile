FROM httpd
ARG UID=1000
ARG GID=1000
RUN usermod -u $UID www-data
RUN groupmod -g $GID www-data
RUN apt-get update && apt-get install -y build-essential && apt-get install -y inotify-tools && rm -rf /var/lib/apt/lists/*
