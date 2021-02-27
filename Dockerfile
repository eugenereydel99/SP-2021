FROM debian
COPY script.sh .
RUN chmod ugo+x script.sh && mkdir documents && touch documents/test.txt
CMD ./script.sh

