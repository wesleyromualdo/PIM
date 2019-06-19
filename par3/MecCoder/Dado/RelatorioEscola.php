<?php
namespace Simec\Par3\Dado;

/**
 * Classe de representação do relatório de escolas
 * @tabela par3.tabela
 */
class RelatorioEscola extends \Simec\AbstractDado
{

    use \Simec\TraitValidadorDado;
    use \Simec\TraitValidadorTipoDado;

    /**
     * Código da escola no INEP
     * @campo inep
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 5
     * @validador numerico
     * @var string
     */
    public $inep;
    public $municipio;

    /**
     * Código da UF
     * @campo uf
     * @tipo texto
     * @obrigatorio true
     * @tamanho 2
     * @validador uf
     * @var string
     */
    public $uf;
    public $ibge;
    public $esfera;
    /**
     * Nome da escola
     * @campo nm_escola
     * @tipo texto
     * @obrigatorio true
     * @tamanho 100
     * @var string
     */
    public $nome;
    public $situacao;
    public $localizacao;
    public $salasUtilizadas;
    public $totalAlunos;
    public $imovel;
    public $endereco;
    public $cep;
    public $coordenadas;
    public $totalDeSalas;
    public $alunosInfantil;
    public $alunosFundamental;
    public $alunosMedio;
    public $escid;

}
