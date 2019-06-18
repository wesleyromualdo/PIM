<?php
/**
 * Agregador de Regra
 * Classe utilizada para validar um lote de regra (Simec_Regra_Abstract)
 *
 * Descricao de exemplo de utilizacao
 *
 * O agregador de classe de validacao de premissa, deve ser criado atraves de uma sub classe de Simec_Regra_Agregador
 *
 * @package Simec\regra
 * @example
 * <code>
 *
 * // -- Exemplo 1
 * $parametro = array(
 *     'usucpf' => '',
 *     'sisid' => '194',
 *     'usutoken' => '@#(*!J@LJsdfw2344Bs5sdfs2342DN!Q*YE(@!HEJKNDA<S+_`DLSKsdfjl',
 * );
 * </code>
 *
 * @example
 * <code>
 * // -- Pode validar regra por sessao/categoriazao passando a chave do array de regras no construtor do Agregador
 * $motor = new Ted_Regra_Agregador('VALIDACAO_DE_PERFIL');
 * foreach ($motor as $classeValidacao) :
 *     $classeValidacao->populaParams($parametro);
 * endforeach;
 *
 * // -- Exemplo 2
 * // -- Instanciando agregador de regras
 * $motor = new Ted_Regra_Agregador('VALIDACAO_DE_PERFIL');
 *
 * // -- Validando uma sequencia de classes em um agregador de regras
 * if (Simec_Regra_Motor::check($motor, $parametro)) {
 *     echo 'Todas as validacoes de regras foram atendidas';
 * } else {
 *     echo 'Alguma das premissas de validacao falhou';
 * }
 *
 * </code>
 *
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
abstract class Simec_Regra_Agregador implements Iterator
{
    private $index;

    private $agregador;

    private $regraFalha;

    const DEFAULT_AGREGADOR = '*';

    /**
     * Recebe as classes de regras
     * @var array
     */
    protected $regras = [];

    public function __construct($agregador = self::DEFAULT_AGREGADOR)
    {
        $this->index = 0;
        $this->init();
        $this->agregador = $agregador;
    }

    /**
     * Inicializador
     */
    public function init()
    {}

    /**
     * Retorna a regra que falhou
     * @return mixed
     */
    public function getRegraFalha()
    {
        return $this->regraFalha;
    }

    /**
     * Guarda referencia da regra que falhou
     * @param $classfail
     */
    public function setRegraFalha(Simec_Regra_Abstract $objectFail)
    {
        $this->regraFalha = get_class($objectFail);
    }

    public function rewind()
    {
        $this->index = 0;
    }

    public function current()
    {
        return $this->regras[$this->agregador][$this->index];
    }

    public function key()
    {
        return $this->index;
    }

    public function next()
    {
        return ++$this->index;
    }

    public function valid()
    {
        return array_key_exists($this->index, $this->regras[$this->agregador]);
    }
}
