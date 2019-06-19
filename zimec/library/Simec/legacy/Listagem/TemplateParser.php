<?php
/**
 * Implementação da classe de parser de templates da listagem.
 *
 * @version $Id: Listagem.php 111319 2016-05-20 18:25:51Z maykelbraz $
 * @filesource
 */

/**
 * Essa classe toma um template e identifica suas repetições e substituições, além de executar cada uma delas.
 *
 * @author Maykel S. Braz <maykel.braz@mec.gov.br>
 */
class Simec_Listagem_TemplateParser
{
    protected $template;
    protected $trechoRepeticao;
    protected $trechoRepeticaoCompleto;

    protected $tags;

    public function __construct($template)
    {
        $this->template = $template;
        $this->encontraRepeticao();
    }

    protected function getTemplate()
    {
        return $this->template;
    }

    protected function getRepeticao()
    {
        if (!isset($this->trechoRepeticao)) {
            $this->encontraRepeticao();
        }

        return $this->trechoRepeticao;
    }

    protected function getRepeticaoCompleto()
    {
        if (!isset($this->trechoRepeticaoCompleto)) {
            $this->encontraRepeticao();
        }

        return $this->trechoRepeticaoCompleto;
    }

    protected function getTags()
    {
        if (!isset($this->tags)) {
            preg_match_all('/%%(\w+)%%/', $this->trechoRepeticao, $matches);
            $this->tags = $matches[1];
        }

        return $this->tags;
    }

    protected function encontraRepeticao()
    {
        preg_match(
            '/{{repetir}}(.*){{fim-repetir}}/s',
            $this->getTemplate(),
            $repeticao
        );

        list($this->trechoRepeticaoCompleto, $this->trechoRepeticao) = $repeticao;
    }

    public function renderRepeticao(array $dados)
    {
        $html = '';

        $dadosSubstituicao = $patterns = array();
        foreach ($this->getTags() as $tag) {
            $dadosSubstituicao[] = simec_htmlentities($dados[$tag]);
            $patterns[] = "%%{$tag}%%";
        }

        return str_replace($patterns, $dadosSubstituicao, $this->getRepeticao());
    }

    public function replace($bloco)
    {
        return str_replace($this->getRepeticaoCompleto(), $bloco, $this->getTemplate());
    }
}
