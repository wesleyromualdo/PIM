<?php


include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

/**
 * Sase_Model_Planocarreiraprofessor
 *
 * @category Class
 * @package  A1
 * @author   VICTOR MARTINS MACHADO <VictorMachado@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 1.0
 * @link     no link
 */
class Sase_Model_Planocarreiraprofessor extends Modelo
{
    /**
     * @var FilesSimec ReferÃªncia para a classe de gestÃ£o de arquivos.
     */
    protected $arquivo;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.planocarreiraprofessor';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pcpid',
    );


    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pcpid' => null,
        'estuf' => null,
        'muncod' => null,
        'usucpf' => null,
        'docid' => null,
        'pcpstatus' => null,
        'docid2' => null,
        'pcppcr' => null,
        'pcpadesao' => null,
        'pcpato' => null,
        'pcpmpl' => null
    );

    /**
     * Retorna uma referencia para FilesSimec.
     *
     * @return FilesSimec
     */
    protected function getArquivo()
    {
        if (is_null($this->arquivo)) {
            $this->arquivo = new FilesSimec();
        }

        return $this->arquivo;
    }

    /**
     * Retorna informaï¿½ï¿½es de um arquivo.
     *
     * @param string $campoArquivo Nome do atributo que guarda o ID do arquivo.
     * @return array
     */
    public function getInfoArquivo($campoArquivo)
    {
        if (empty($this->$campoArquivo)) {
            return [];
        }
        return $this
            ->getArquivo()
            ->getDadosArquivo($this->$campoArquivo);
    }
}
