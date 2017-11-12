% Apply L2-normalization to each tile. Each tile contains as many values as
% the number of operators
function [train_feats, test_feats] = Normalize_Data(train_feats, test_feats, vocab_size)
fun = @(x) normr(x);

% tf = extractfield(train_feats,'bags');
% tf = vec2mat(tf,2100);
% 
% tf = blkproc(tf,[size(tf,1),vocab_size],fun);
% 
% for i = 1:size(tf,1)
%     train_feats(i).bags = tf(i,:);
% end
train_feats = blkproc(train_feats,[size(train_feats,1),vocab_size],fun);
test_feats = blkproc(test_feats,[size(test_feats,1),vocab_size],fun);